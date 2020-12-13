#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include "Imgui/imgui.h"


Application::~Application() {}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	//Initializes the Graphics object
	pGfx = new Graphics();
	pGfx->Initialise(hInstance, nCmdShow);

	//Initializes the ImGUIManager and the JSON level loader
	mImGuiManager = ImGUIManager(pGfx);
	mJSONLevelLoader = JSONLevelLoader(pGfx);

	//Sets the game objects vector and loads the objects from the json file
	pGameObjects = mJSONLevelLoader.LoadObjectsFromFile("Assets/Levels/level.json");

	//Finds the cube and donut objects from the game objects array, used so these can be referenced in code
	pCube = FindGameObjectWithName("Cube");
	pDonut = FindGameObjectWithName("Donut");



	//Initializes the Ground Plane object and creates the geometry
	pGroundPlane = new Plane(pGfx);
	pGroundPlane->Make(75.0, 75.0f, 8, 8);

	//Sets default positions
	pGroundPlane->SetPosition(0.0f, -1.6f, 0.0f);

	//Loads the texture for the ground plane
	pGroundPlane->CreateTexture(L"Assets/Textures/stone.dds");

	//Adds the ground plane to the game objects vector so it will be rendered and updated
	pGameObjects.push_back(pGroundPlane);



	//Initializes all camera objects
	pCameraA = std::make_shared<Camera>();
	pCameraB = std::make_shared<Camera>();
	pCameraC = std::make_shared<Camera>();
	pCameraD = std::make_shared<Camera>();

	//Sets the current camera and sends it to the graphics class
	pCurrentCamera = pCameraA;
	pGfx->SetCurrentCamera(pCameraA);

	//Sets the default positions and look at targets for the cameras
	pCameraA->LookAt(XMFLOAT3(0.0f, 8.0f, -15.0f),XMFLOAT3(0.0f, 0.0f, 0.0f));
	pCameraB->LookAt(XMFLOAT3(0.0f, 4.5f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	pCameraC->LookAt(XMFLOAT3(0.0f, 25.0f, -0.1f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	pCameraD->LookAt(XMFLOAT3(-25.0f, 25.0f, -25.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));


	//Initializes the skysphere and scales it up
	pSkySphere = new MeshedObject(pGfx, "Assets/Models/sphere.obj");
	pSkySphere->SetScale(500.0f, 500.0f, 500.0f);

	//Loads the skybox texture and sets the shader
	pSkySphere->CreateTexture(L"Assets/Textures/Skybox.dds");
	pSkySphere->SetShader(L"Skybox.fx");

	//Initializes the transparent cube object, and positions it
	pBlendedCube = new MeshedObject(pGfx, "Assets/Models/cube.obj");
	pBlendedCube->SetPosition(4.0f, 1.2f, 3.0f);

	//Sets the material, texture and shader
	pBlendedCube->SetMaterialDiffuse(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
	pBlendedCube->CreateTexture(L"Assets/Textures/Crate_COLOR.dds");
	pBlendedCube->SetShader(L"PhongDif.fx");

	//Initializes the bounding sphere collider
	pBlendedCube->InitializeBoundingSphere();


	//Initializes and resets the Timer object
	mTime = Time();
	mTime.Reset();

	return S_OK;
}

void Application::Update()
{
	//Ticks the Timer object and gets the new delta time value
	mTime.Tick();
	float dt = mTime.DeltaTime();

	//Polls the users input
	PollInput(dt);

	//Camera B Selected Object Tracking 
	//Checks that a selected object is available and that Camera B is the current camera
	if (pSelectedObject != nullptr && pCurrentCamera == pCameraB) {
		//Gets the selected objects position
		XMFLOAT3 selectedObjectPosition = pSelectedObject->GetPosition();

		//Calculates the camera position with the camera offset added to it
		XMFLOAT3 newCameraBPos = XMFLOAT3
		(
			selectedObjectPosition.x + mCameraBOffset.x,
			selectedObjectPosition.y + mCameraBOffset.y,
			selectedObjectPosition.z + mCameraBOffset.z
		);

		//Uses the look at method to reposition camera b and aim it at the selected object
		pCameraB->LookAt
		(
			newCameraBPos,
			selectedObjectPosition
		);
	}

	//Calculates the new rotation value 
	mObjectRotationValue += (mObjectRotationSpeed * dt);

	//Sets the rotation of the cube and the donut game objects
	pCube->SetRotation(10.0f, mObjectRotationValue, 0.0f);
	pDonut->SetRotation(mObjectRotationValue, 0.0f, 0.0f);

	//Updates the view matrix for the current camera
	pCurrentCamera->UpdateViewMatrix();


	//Loops through every game object and calls the update method
	for (auto& object : pGameObjects) {
		object->Update(dt);
	}

	//Updates the blended cube object
	pBlendedCube->Update(dt);


	//Sets the position of the sky sphere to the cameras position so that the camera cant fly out of the sky
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
		if (originalWireframe != mWireframeEnabled) pGfx->SetWireframe(mWireframeEnabled);

		//Background Clear Color 
		ImGui::Text("Background Clear Colour: ");
		ImGui::NewLine();
		ImGui::PushItemWidth(100.0f);
		ImGui::ColorPicker4("####", mClearColor);
		pGfx->SetClearColor(mClearColor);

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
			//Object Name		
			const char* name = pSelectedObject->GetName().c_str();
			ImGui::Text(name);
			
			//Object Position
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

	//Calls the lighting control panel method in the graphics class
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
		pGfx->SetCurrentCamera(pCameraA);
	}
	else if (GetAsyncKeyState('2')) {
		pCurrentCamera = pCameraB;
		mFlyingEnabled = false;
		pGfx->SetCurrentCamera(pCameraB);
	}
	else if (GetAsyncKeyState('3')) {
		pCurrentCamera = pCameraC;
		pGfx->SetCurrentCamera(pCameraC);
	}
	else if (GetAsyncKeyState('4')) {
		pCurrentCamera = pCameraD;
		pGfx->SetCurrentCamera(pCameraD);
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
