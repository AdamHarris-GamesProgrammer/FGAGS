#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include "Imgui/imgui.h"



Application::Application() {}

Application::~Application()
{
	delete cameraA;

	cameraA = nullptr;
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	graphics = new Graphics();
	graphics->Initialise(hInstance, nCmdShow);

	groundPlane = new Plane(graphics);
	groundPlane->Make(75.0, 75.0f, 8, 8);


	mImGuiManager = new ImGUIManager(graphics);


	mJSONLevelLoader = new JSONLevelLoader(graphics);

	mGameObjects = mJSONLevelLoader->LoadObjectsFromFile("Assets/Levels/level.json");

	mGameObjects.push_back(groundPlane);

	cube = FindGameObjectWithName("Cube");
	donut = FindGameObjectWithName("Donut");

	groundPlane->CreateTexture(L"Assets/Textures/stone.dds");


	cameraA = new Camera();
	cameraA->SetLens(0.25f * 3.1452, graphics->GetWindowWidth() / graphics->GetWindowHeight(), 0.01f, 100.0f);

	mCurrentCamera = cameraA;
	graphics->SwitchCamera(cameraA);

	cameraB = new Camera();

	cameraA->LookAt(
		XMFLOAT3(0.0f, 8.0f, -15.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f)
	);

	cameraA->UpdateViewMatrix();

	//Sets default positions
	groundPlane->SetPosition(0.0f, -1.6f, 0.0f);

	rotationValue = 0.0f;

	time = Time();

	time.Reset();


	return S_OK;
}

void Application::Update()
{
	time.Tick();
	float dt = time.DeltaTime();

	PollInput(dt);

	//Camera B Selected Object Tracking 
	if (mSelectedObject != nullptr && mCurrentCamera == cameraB) {
		XMFLOAT3 selectedObjectPosition = mSelectedObject->GetPosition();
		XMFLOAT3 newCameraBPos = XMFLOAT3
		(
			selectedObjectPosition.x + cameraBOffset.x,
			selectedObjectPosition.y + cameraBOffset.y,
			selectedObjectPosition.z + cameraBOffset.z
		);

		cameraB->LookAt
		(
			newCameraBPos,
			selectedObjectPosition
		);
	}


	rotationValue += (rotationSpeed * dt);

	cube->SetRotation(10.0f, rotationValue, 0.0f);
	donut->SetRotation(rotationValue, 0.0f, 0.0f);

	mCurrentCamera->UpdateViewMatrix();

	for (auto& object : mGameObjects) {
		object->Update(dt);
	}
}

void Application::SelectedObjectControl(float dt)
{
	XMFLOAT3 objectPosition = mSelectedObject->GetPosition();

	if (GetAsyncKeyState('W')) {
		objectPosition.z += movementSpeed * dt;
	}
	else if (GetAsyncKeyState('S')) {
		objectPosition.z += -movementSpeed * dt;
	}
	if (GetAsyncKeyState('A')) {
		objectPosition.x += -movementSpeed * dt;
	}
	else if (GetAsyncKeyState('D')) {
		objectPosition.x += movementSpeed * dt;
	}

	//if the position of the selected object changed then update the new position
	if (objectPosition.x != mSelectedObject->GetPosition().x
		|| objectPosition.z != mSelectedObject->GetPosition().z) {
		mSelectedObject->SetPosition(objectPosition);
	}
}

void Application::DrawGUI()
{
	//Simulation Settings Window
	{
		bool originalWireframe = wireframeOn;

		bool* open = new bool(true);

		//Opens a Window called "Simulation Settings, which defaults to open and is non re sizable
		ImGui::Begin("Simulation Settings", open, ImGuiWindowFlags_NoResize);
		
		//Object Movement Speed
		ImGui::Text("Object Movement Speed: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::SliderFloat("", &movementSpeed, 0.0f, 10.0f);

		//Object Rotation Speed
		ImGui::Text("Object Rotation Speed: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::SliderFloat("##", &rotationSpeed, 0.0f, 15.0f);

		//Wireframe Checkbox
		ImGui::Text("Wireframe Mode: ");
		ImGui::SameLine();
		ImGui::Checkbox("###", &wireframeOn);
		if (originalWireframe != wireframeOn) graphics->EnableWireframe(wireframeOn);

		//Background Clear Color 
		ImGui::Text("Background Clear Colour");
		ImGui::NewLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::ColorPicker4("####", clearColor);
		graphics->SetClearColor(clearColor);

		ImGui::End();
	}



	//Selected Object Window

}

void Application::Picking()
{
	int mouseX = graphics->GetMouseX();
	int mouseY = graphics->GetMouseY();

	XMMATRIX invView = XMMatrixInverse(nullptr, mCurrentCamera->View());
	XMMATRIX invProj = XMMatrixInverse(nullptr, mCurrentCamera->Proj());

	//Convert mouse position to NDC (Normalized Device Coordinates)
	float normalizedCoords[2];
	normalizedCoords[0] = (2.0f * mouseX) / graphics->GetWindowWidth() - 1.0f;
	normalizedCoords[1] = 1.0f - (2.0f * mouseY) / graphics->GetWindowHeight();

	//Sets the X and Y positions for the origin of the ray
	XMVECTOR rayOrigin = XMVectorSet(normalizedCoords[0], normalizedCoords[1], 0, 0);

	//Converts it from screen space to projection space
	rayOrigin = XMVector3Transform(rayOrigin, invProj);

	//Converts from screen space to view space
	rayOrigin = XMVector3Transform(rayOrigin, invView);

	//Calculates the direction of the ray
	XMVECTOR rayDirection = rayOrigin - XMLoadFloat3(&cameraA->GetPosition());

	//Normalizes the direction
	rayDirection = XMVector3Normalize(rayDirection);

	//Stores them as a XMFLOAT4 to allow passing between functions
	XMFLOAT4 origin;
	XMFLOAT4 direction;
	XMStoreFloat4(&origin, rayOrigin);
	XMStoreFloat4(&direction, rayDirection);

	bool objectFound = false;

	//Loops through all objects in the scene 
	for (auto& object : mGameObjects) {
		//Tests collision with each object
		if (object->TestCollision(origin, direction)) {
			//Sets the selected object
			mSelectedObject = object;

			objectFound = true;

			//Breaks when a object has been selected, stops it from selecting objects behind the original object
			break;
		}
	}

	//Allows user to deselect an item
	if (!objectFound) mSelectedObject = nullptr;
}

void Application::PollInput(float dt)
{
	CursorControls(dt);

	if (enableFlying)
	{
		CameraControls(dt);
	}
	else
	{
		if (mSelectedObject != nullptr) {
			SelectedObjectControl(dt);
		}
	}

	if (GetAsyncKeyState('1')) {
		mCurrentCamera = cameraA;
		graphics->SwitchCamera(cameraA);
	}
	else if (GetAsyncKeyState('2')) {
		mCurrentCamera = cameraB;
		enableFlying = false;
		graphics->SwitchCamera(cameraB);
	}

	if (GetAsyncKeyState('H')) {
		Picking();
	}
	if (GetAsyncKeyState('Y')) {
		mSelectedObject = nullptr;
	}
}

void Application::CursorControls(float dt)
{
	timeSinceGPressed += dt;
	if (GetAsyncKeyState('G')) {
		if (timeSinceGPressed > fTimer) {
			timeSinceGPressed = 0.0f;

			enableFlying = !enableFlying;

			clippedCursor = !clippedCursor;
			if (clippedCursor) {
				graphics->HideCursor();
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			}
			else
			{
				graphics->ShowCursor();
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			}
		}
	}
}

void Application::CameraControls(float dt)
{
	if (GetAsyncKeyState('W')) cameraA->Walk(10.0f * dt);
	else if (GetAsyncKeyState('S')) cameraA->Walk(-10.0f * dt);
	if (GetAsyncKeyState('A')) cameraA->Strafe(-10.0f * dt);
	else if (GetAsyncKeyState('D')) cameraA->Strafe(10.0f * dt);

	if (GetAsyncKeyState('Q')) cameraA->RotateY(-1.0f * dt);
	else if (GetAsyncKeyState('E')) cameraA->RotateY(1.0f * dt);

	if (GetAsyncKeyState('R')) cameraA->Pitch(-1.0f * dt);
	else if (GetAsyncKeyState('F')) cameraA->Pitch(1.0f * dt);
}

void Application::Draw()
{
	mImGuiManager->BeginFrame();

	graphics->ClearBuffers();

	for (auto& object : mGameObjects) {
		object->Draw();
	}

	ImGui::ShowDemoWindow();


	DrawGUI();

	mImGuiManager->EndFrame();

	graphics->Present();

}

GameObject* Application::FindGameObjectWithName(std::string name)
{
	for (auto& obj : mGameObjects) {
		if (obj->GetName() == name) return obj;
	}
}
