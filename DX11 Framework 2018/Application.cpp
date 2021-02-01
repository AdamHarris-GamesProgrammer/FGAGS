#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include "Imgui/imgui.h"


Application::~Application() 
{
	pGameObjects.clear();

	delete pGroundPlane;
	delete pGfx;

	pGroundPlane = nullptr;
	pGfx = nullptr;
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	//Initializes the Graphics object
	pGfx = new Graphics();
	pGfx->Initialise(hInstance, nCmdShow);

	//Initializes the ImGUIManager and the JSON level loader
	mJSONLevelLoader = JSONLevelLoader(pGfx);

	//Sets the game objects vector and loads the objects from the json file
	pGameObjects = mJSONLevelLoader.LoadObjectsFromFile("Assets/Levels/level.json");

	//Finds the cube and donut objects from the game objects array, used so these can be referenced in code
	pCube = FindGameObjectWithName("Cube");
	pDonut = FindGameObjectWithName("Donut");


	pCameras = mJSONLevelLoader.LoadCamerasFromFile("Assets/Levels/level.json");
	pCurrentCamera = pCameras[0];
	pGfx->SetCurrentCamera(pCurrentCamera);


	//Initializes the skysphere and scales it up
	std::string filepath = "Assets/Textures/Skybox.dds";
	pSkySphere = new SkySphere(pGfx, filepath);



	//Initializes the Ground Plane object and creates the geometry
	pGroundPlane = new Plane(pGfx);
	pGroundPlane->Make(75.0, 75.0f, 8, 8);

	//Sets default positions
	pGroundPlane->GetTransform().SetPosition(0.0f, -1.6f, 0.0f);

	//Loads the texture for the ground plane
	pGroundPlane->CreateTexture(L"Assets/Textures/stone.dds");

	//Adds the ground plane to the game objects vector so it will be rendered and updated
	pGameObjects.push_back(pGroundPlane);

	//Initializes the transparent cube object, and positions it
	pBlendedCube = new MeshedObject(pGfx, "Assets/Models/cube.obj");
	pBlendedCube->GetTransform().SetPosition(4.0f, 1.2f, 3.0f);


	//Sets the material, texture and shader
	pBlendedCube->GetMaterial().SetMaterialDiffuse(XMFLOAT4(1.0f, 0.0f, 0.0f, 0.9f));
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
	if (pSelectedObject != nullptr && pCurrentCamera == pCameras[1]) {
		//Gets the selected objects position
		Vector3 selectedObjectPosition = pSelectedObject->GetTransform().GetPosition();

		//Uses the look at method to reposition camera b and aim it at the selected object
		pCameras[1]->LookAt
		(
			selectedObjectPosition + mCameraBOffset,
			selectedObjectPosition
		);
	}

	//Calculates the new rotation value 
	mObjectRotationValue += (mObjectRotationSpeed * dt);

	//Sets the rotation of the cube and the donut game objects
	pCube->GetTransform().SetRotation(10.0f, mObjectRotationValue, 0.0f);
	pDonut->GetTransform().SetRotation(mObjectRotationValue, 0.0f, 0.0f);

	//Updates the view matrix for the current camera
	pCurrentCamera->UpdateViewMatrix();


	//Loops through every game object and calls the update method
	for (auto& object : pGameObjects) {
		object->Update(dt);
	}

	//Updates the blended cube object
	pBlendedCube->Update(dt);

	pSkySphere->Update(dt);
}

void Application::SelectedObjectControl(float dt)
{
	Vector3 objectPosition = pSelectedObject->GetTransform().GetPosition();

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
	if (objectPosition.x != pSelectedObject->GetTransform().GetPosition().x
		|| objectPosition.z != pSelectedObject->GetTransform().GetPosition().z) {
		pSelectedObject->GetTransform().SetPosition(objectPosition);
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

		//E3
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

		//C3
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
				pSelectedObject->GetTransform().GetPosition().x,
				pSelectedObject->GetTransform().GetPosition().y,
				pSelectedObject->GetTransform().GetPosition().z,
			};

			ImGui::SliderFloat3("Position", pos, -100.0f, 100.0f);
			pSelectedObject->GetTransform().SetPosition(pos[0], pos[1], pos[2]);
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
		pCurrentCamera = pCameras[0];
		pGfx->SetCurrentCamera(pCurrentCamera);
	}
	else if (GetAsyncKeyState('2')) {
		pCurrentCamera = pCameras[1];
		mFlyingEnabled = false;
		pGfx->SetCurrentCamera(pCurrentCamera);
	}
	else if (GetAsyncKeyState('3')) {
		pCurrentCamera = pCameras[2];
		pGfx->SetCurrentCamera(pCurrentCamera);
	}
	else if (GetAsyncKeyState('4')) {
		pCurrentCamera = pCameras[3];
		pGfx->SetCurrentCamera(pCurrentCamera);
	}

	if (GetAsyncKeyState('5'))
	{
		pBlendedCube->GetParticle()->SetVelocity(0.0, 30.0, 40.0);
		pBlendedCube->GetParticle()->SetAcceleration(0.0, -20.0, 0.0);
		pBlendedCube->GetParticle()->SetMass(20.0);
		pBlendedCube->GetParticle()->SetDamping(0.9);
	}
	if (GetAsyncKeyState('6')) {
		pBlendedCube->GetParticle()->SetVelocity();
		pBlendedCube->GetParticle()->SetAcceleration();
		pBlendedCube->GetParticle()->SetMass(0.1);
		pBlendedCube->GetParticle()->SetDamping(1.0);
		pBlendedCube->GetTransform().SetPosition(4.0, 1.2, 0.0);
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
	if (GetAsyncKeyState('W')) pCameras[0]->Walk(10.0f * dt);
	else if (GetAsyncKeyState('S')) pCameras[0]->Walk(-10.0f * dt);
	if (GetAsyncKeyState('A')) pCameras[0]->Strafe(-10.0f * dt);
	else if (GetAsyncKeyState('D')) pCameras[0]->Strafe(10.0f * dt);

	if (GetAsyncKeyState('Q')) pCameras[0]->RotateY(-1.0f * dt);
	else if (GetAsyncKeyState('E')) pCameras[0]->RotateY(1.0f * dt);

	if (GetAsyncKeyState('R')) pCameras[0]->Pitch(-1.0f * dt);
	else if (GetAsyncKeyState('F')) pCameras[0]->Pitch(1.0f * dt);
}

void Application::Draw()
{
	pGfx->BeginFrame();

	pSkySphere->Draw();

	for (auto& object : pGameObjects) {
		object->Draw();
	}

	pGfx->SetTransparentBlend();
	pBlendedCube->Draw();

	DrawGUI();

	pGfx->EndFrame();
}

GameObject* Application::FindGameObjectWithName(std::string name)
{
	for (auto& obj : pGameObjects) {
		if (obj->GetName() == name) return obj;
	}

	return nullptr;
}
