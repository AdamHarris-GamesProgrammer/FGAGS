#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include "Imgui/imgui.h"


Application::~Application()
{
	delete pCameraA;

	pCameraA = nullptr;
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	pGfx = new Graphics();
	pGfx->Initialise(hInstance, nCmdShow);

	pGroundPlane = new Plane(pGfx);
	pGroundPlane->Make(75.0, 75.0f, 8, 8);


	mImGuiManager = ImGUIManager(pGfx);
	mJSONLevelLoader = JSONLevelLoader(pGfx);

	pGameObjects = mJSONLevelLoader.LoadObjectsFromFile("Assets/Levels/level.json");

	pGameObjects.push_back(pGroundPlane);

	pCube = FindGameObjectWithName("Cube");
	pDonut = FindGameObjectWithName("Donut");

	pGroundPlane->CreateTexture(L"Assets/Textures/stone.dds");


	pCameraA = new Camera();
	pCameraA->SetLens(0.25f * 3.1452f, pGfx->GetWindowWidth() / pGfx->GetWindowHeight(), 0.01f, 600.0f);

	pCurrentCamera = pCameraA;
	pGfx->SwitchCamera(pCameraA);

	pCameraB = new Camera();
	pCameraB->SetLens(0.25f * 3.1452f, pGfx->GetWindowWidth() / pGfx->GetWindowHeight(), 0.01f, 100.0f);

	pCameraA->LookAt(
		XMFLOAT3(0.0f, 8.0f, -15.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f)
	);

	pCameraC = new Camera();
	pCameraC->LookAt(XMFLOAT3(0.0f, 25.0f, -0.1f), XMFLOAT3(0.0f, 0.0f, 0.0f));


	pCameraD = new Camera();
	pCameraD->LookAt(XMFLOAT3(-25.0f, 25.0f, -25.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

	pCameraA->UpdateViewMatrix();
	pCameraB->UpdateViewMatrix();
	pCameraC->UpdateViewMatrix();
	pCameraD->UpdateViewMatrix();


	pSkySphere = new MeshedObject(pGfx, "Assets/Models/sphere.obj");
	pSkySphere->CreateTexture(L"Assets/Textures/Skybox.dds");
	pSkySphere->SetShader(L"Skybox.fx");
	pSkySphere->SetScale(500.0f, 500.0f, 500.0f);

	pBlendedCube = new MeshedObject(pGfx, "Assets/Models/cube.obj");
	pBlendedCube->SetPosition(4.0f, 1.2f, 3.0f);
	pBlendedCube->SetMaterialDiffuse(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	pBlendedCube->CreateTexture(L"Assets/Textures/Crate_COLOR.dds");
	pBlendedCube->SetShader(L"PhongDif.fx");
	pBlendedCube->InitializeBoundingSphere();



	//Sets default positions
	pGroundPlane->SetPosition(0.0f, -1.6f, 0.0f);

	mObjectRotationValue = 0.0f;

	mTime = Time();

	mTime.Reset();


	return S_OK;
}

void Application::Update()
{
	mTime.Tick();
	float dt = mTime.DeltaTime();

	PollInput(dt);

	//Camera B Selected Object Tracking 
	if (pSelectedObject != nullptr && pCurrentCamera == pCameraB) {
		XMFLOAT3 selectedObjectPosition = pSelectedObject->GetPosition();
		XMFLOAT3 newCameraBPos = XMFLOAT3
		(
			selectedObjectPosition.x + mCameraBOffset.x,
			selectedObjectPosition.y + mCameraBOffset.y,
			selectedObjectPosition.z + mCameraBOffset.z
		);

		pCameraB->LookAt
		(
			newCameraBPos,
			selectedObjectPosition
		);
	}


	mObjectRotationValue += (mObjectRotationSpeed * dt);

	pCube->SetRotation(10.0f, mObjectRotationValue, 0.0f);
	pDonut->SetRotation(mObjectRotationValue, 0.0f, 0.0f);

	pCurrentCamera->UpdateViewMatrix();

	for (auto& object : pGameObjects) {
		object->Update(dt);
	}

	pBlendedCube->Update(dt);


	pSkySphere->SetPosition(pCurrentCamera->GetPosition());
	pSkySphere->Update(dt);
}

void Application::SelectedObjectControl(float dt)
{
	XMFLOAT3 objectPosition = pSelectedObject->GetPosition();

	if (GetAsyncKeyState('W')) {
		objectPosition.z += mObjectMovementSpeed * dt;
	}
	else if (GetAsyncKeyState('S')) {
		objectPosition.z += -mObjectMovementSpeed * dt;
	}
	if (GetAsyncKeyState('A')) {
		objectPosition.x += -mObjectMovementSpeed * dt;
	}
	else if (GetAsyncKeyState('D')) {
		objectPosition.x += mObjectMovementSpeed * dt;
	}

	//if the position of the selected object changed then update the new position
	if (objectPosition.x != pSelectedObject->GetPosition().x
		|| objectPosition.z != pSelectedObject->GetPosition().z) {
		pSelectedObject->SetPosition(objectPosition);
	}
}

void Application::DrawGUI()
{
	//Simulation Settings Window
	{
		bool originalWireframe = mWireframeEnabled;

		bool* open = new bool(true);

		//Opens a Window called "Simulation Settings, which defaults to open and is non re sizable
		ImGui::Begin("Simulation Settings", open, ImGuiWindowFlags_NoResize);

		//Object Movement Speed
		ImGui::Text("Object Movement Speed: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::SliderFloat("", &mObjectMovementSpeed, 0.0f, 10.0f);

		//Object Rotation Speed
		ImGui::Text("Object Rotation Speed: ");
		ImGui::SameLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::SliderFloat("##", &mObjectRotationSpeed, 0.0f, 15.0f);

		//Wireframe Checkbox
		ImGui::Text("Wireframe Mode: ");
		ImGui::SameLine();
		ImGui::Checkbox("###", &mWireframeEnabled);
		if (originalWireframe != mWireframeEnabled) pGfx->EnableWireframe(mWireframeEnabled);

		//Background Clear Color 
		ImGui::Text("Background Clear Colour: ");
		ImGui::NewLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::ColorPicker4("####", clearColor);
		pGfx->SetClearColor(clearColor);

		ImGui::End();
	}

	//Controls Window
	{
		ImGui::Begin("Controls");

		//Camera A Controls
		ImGui::Text("Camera A (Debug Fly Cam): 1");
		ImGui::Text("Camera A Enable Movement: G");
		if (ImGui::TreeNode("Camera A Movement Controls")) {
			ImGui::Text("Walk Forwards: W");
			ImGui::Text("Walk Backwards: S");
			ImGui::Text("Walk Left: A");
			ImGui::Text("Walk Right: D");
			ImGui::Text("Rotate Left: Q");
			ImGui::Text("Rotate Right: E");
			ImGui::Text("Pitch Up: R");
			ImGui::Text("Pitch Down: F");
			ImGui::TreePop();
		}
		ImGui::Text("Camera B (Third Person Cam): 2");
		ImGui::Text("Camera C (Top Down Cam): 3");
		ImGui::Text("Camera D (Level Overview): 4");

		//Selected Object Controls
		ImGui::Text("Select Object with Mouse Position: H");
		if (ImGui::TreeNode("Selected Object Controls")) {
			ImGui::Text("Move Forwards: W");
			ImGui::Text("Move Backwards: S");
			ImGui::Text("Move Left: A");
			ImGui::Text("Move Right: D");
			ImGui::Text("Press Y to deselect current object");
			ImGui::TreePop();
		}


		ImGui::End();
	}
	
	//Selected Object Window
	{
		ImGui::Begin("Selected Object");

		if (pSelectedObject != nullptr) {
			const char* name = pSelectedObject->GetName().c_str();

			ImGui::Text(name);
			
			float pos[3] = {
				pSelectedObject->GetPosition().x,
				pSelectedObject->GetPosition().y,
				pSelectedObject->GetPosition().z,
			};

			ImGui::SliderFloat3("Position", pos, -100.0f, 100.0f);

			pSelectedObject->SetPosition(pos[0], pos[1], pos[2]);
		}

		ImGui::End();
	}

	pGfx->LightingWindow();
}

void Application::Picking()
{
	int mouseX = pGfx->GetMouseX();
	int mouseY = pGfx->GetMouseY();

	XMMATRIX invView = XMMatrixInverse(nullptr, pCurrentCamera->View());
	XMMATRIX invProj = XMMatrixInverse(nullptr, pCurrentCamera->Proj());

	//Convert mouse position to NDC (Normalized Device Coordinates)
	float normalizedCoords[2];
	normalizedCoords[0] = (2.0f * mouseX) / pGfx->GetWindowWidth() - 1.0f;
	normalizedCoords[1] = 1.0f - (2.0f * mouseY) / pGfx->GetWindowHeight();

	//Sets the X and Y positions for the origin of the ray
	XMVECTOR rayOrigin = XMVectorSet(normalizedCoords[0], normalizedCoords[1], 0, 0);

	//Converts it from screen space to projection space
	rayOrigin = XMVector3Transform(rayOrigin, invProj);

	//Converts from screen space to view space
	rayOrigin = XMVector3Transform(rayOrigin, invView);

	//Calculates the direction of the ray
	XMVECTOR rayDirection = rayOrigin - XMLoadFloat3(&pCurrentCamera->GetPosition());

	//Normalizes the direction
	rayDirection = XMVector3Normalize(rayDirection);

	//Stores them as a XMFLOAT4 to allow passing between functions
	XMFLOAT4 origin;
	XMFLOAT4 direction;
	XMStoreFloat4(&origin, rayOrigin);
	XMStoreFloat4(&direction, rayDirection);

	bool objectFound = false;

	//Loops through all objects in the scene 
	for (auto& object : pGameObjects) {
		//Tests collision with each object
		if (object->TestCollision(origin, direction)) {
			//Sets the selected object
			pSelectedObject = object;

			objectFound = true;

			//Breaks when a object has been selected, stops it from selecting objects behind the original object
			break;
		}
	}

	//Allows user to deselect an item
	if (!objectFound) pSelectedObject = nullptr;
}

void Application::PollInput(float dt)
{
	CursorControls(dt);

	if (mFlyingEnabled)
	{
		CameraControls(dt);
	}
	else
	{
		if (pSelectedObject != nullptr) {
			SelectedObjectControl(dt);
		}
	}

	if (GetAsyncKeyState('1')) {
		pCurrentCamera = pCameraA;
		pGfx->SwitchCamera(pCameraA);
	}
	else if (GetAsyncKeyState('2')) {
		pCurrentCamera = pCameraB;
		mFlyingEnabled = false;
		pGfx->SwitchCamera(pCameraB);
	}
	else if (GetAsyncKeyState('3')) {
		pCurrentCamera = pCameraC;
		pGfx->SwitchCamera(pCameraC);
	}
	else if (GetAsyncKeyState('4')) {
		pCurrentCamera = pCameraD;
		pGfx->SwitchCamera(pCameraD);
	}

	if (GetAsyncKeyState('H')) {
		Picking();
	}
	if (GetAsyncKeyState('Y')) {
		pSelectedObject = nullptr;
	}
}

void Application::CursorControls(float dt)
{
	mTimeSinceGPressed += dt;
	if (GetAsyncKeyState('G')) {
		if (mTimeSinceGPressed > mGKeyTimer) {
			mTimeSinceGPressed = 0.0f;

			mFlyingEnabled = !mFlyingEnabled;

			mCursorClipped = !mCursorClipped;
			if (mCursorClipped) {
				pGfx->HideCursor();
				ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			}
			else
			{
				pGfx->ShowCursor();
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
			}
		}
	}
}

void Application::CameraControls(float dt)
{
	if (GetAsyncKeyState('W')) pCameraA->Walk(10.0f * dt);
	else if (GetAsyncKeyState('S')) pCameraA->Walk(-10.0f * dt);
	if (GetAsyncKeyState('A')) pCameraA->Strafe(-10.0f * dt);
	else if (GetAsyncKeyState('D')) pCameraA->Strafe(10.0f * dt);

	if (GetAsyncKeyState('Q')) pCameraA->RotateY(-1.0f * dt);
	else if (GetAsyncKeyState('E')) pCameraA->RotateY(1.0f * dt);

	if (GetAsyncKeyState('R')) pCameraA->Pitch(-1.0f * dt);
	else if (GetAsyncKeyState('F')) pCameraA->Pitch(1.0f * dt);
}

void Application::Draw()
{
	mImGuiManager.BeginFrame();

	pGfx->ClearBuffers();

	pGfx->SetConstantBuffer();

	pGfx->SetSolidBlend();

	//Draw Sky sphere
	pGfx->SetFrontFaceCulling();
	pSkySphere->Draw();
	pGfx->SetCurrentRSState();


	for (auto& object : pGameObjects) {
		object->Draw();
	}

	pGfx->SetTransparentBlend();
	pBlendedCube->Draw();

	DrawGUI();

	mImGuiManager.EndFrame();

	pGfx->Present();

}

GameObject* Application::FindGameObjectWithName(std::string name)
{
	for (auto& obj : pGameObjects) {
		if (obj->GetName() == name) return obj;
	}

	return nullptr;
}
