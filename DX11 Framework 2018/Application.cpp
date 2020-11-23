#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

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

	sphere = new Sphere(graphics);
	cube = new Cube(graphics);
	cylinder = new Cylinder(graphics);
	donut = new Donut(graphics);

	groundPlane = new Plane(graphics);
	groundPlane->Make(25.0f, 25.0f, 8, 8);
	
	std::ifstream inFile("Assets/Levels/level.json");
	json jsonFile;

	if (!inFile.is_open()) {
		__debugbreak();
	}

	inFile >> jsonFile;

	json gameobjects = jsonFile["gameobjects"];
	int size = gameobjects.size();


	auto HasAttribute = [](json* file, std::string key) {
		if (file->contains(key)) return true;

		return false;
	};

	for (unsigned int i = 0; i < size; i++) {
		MeshedObject* go = new MeshedObject(graphics);
		json jsonGo = gameobjects.at(i);
		std::string name = jsonGo["name"];
		go->SetName(name);

		if (HasAttribute(&jsonGo, "position")) {
			std::vector<float> position = jsonGo["position"];
			go->SetPosition(position[0], position[1], position[2]);
		}

		if (HasAttribute(&jsonGo, "rotation")) {
			std::vector<float> rotation = jsonGo["rotation"];
			go->SetRotation(rotation[0], rotation[1], rotation[2]);
		}

		if (HasAttribute(&jsonGo, "scale")) {
			std::vector<float> scale = jsonGo["scale"];
			go->SetScale(scale[0], scale[1], scale[2]);
		}

		if (HasAttribute(&jsonGo, "meshPath")) {

			std::string meshName = jsonGo["meshPath"];
			std::string meshPath = "Assets/Models/" + meshName;
			go->Load(meshPath);
		}

		if (HasAttribute(&jsonGo, "diffuseTexture")) {
			std::string textureName = jsonGo["diffuseTexture"];
			std::string path = "Assets/Textures/" + textureName;
			
			wchar_t* convertedPath = new wchar_t[path.length() + 1];
			mbstowcs_s(0, convertedPath, path.length() + 1, path.data(), _TRUNCATE);

			go->CreateTexture(convertedPath);
		}

		if (HasAttribute(&jsonGo, "specularTexture")) {
			std::string textureName = jsonGo["specularTexture"];
			std::string path = "Assets/Textures/" + textureName;

			wchar_t* convertedPath = new wchar_t[path.length() + 1];
			mbstowcs_s(0, convertedPath, path.length() + 1, path.data(), _TRUNCATE);

			go->CreateTexture(convertedPath);
		}

		mGameObjects.push_back(go);
	}

	//__debugbreak();

	mGameObjects.push_back(sphere);
	mGameObjects.push_back(cube);
	mGameObjects.push_back(cylinder);
	mGameObjects.push_back(donut);
	mGameObjects.push_back(groundPlane);

	sphere->CreateTexture(L"Assets/Textures/Crate_COLOR.dds");
	sphere->CreateTexture(L"Assets/Textures/Crate_SPEC.dds");

	cube->CreateTexture(L"Assets/Textures/Marble_COLOR.dds");
	cube->CreateTexture(L"Assets/Textures/Marble_SPEC.dds");

	cylinder->CreateTexture(L"Assets/Textures/Crate_COLOR.dds");
	donut->CreateTexture(L"Assets/Textures/Marble_COLOR.dds");

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
	sphere->SetPosition(-4.0f, 0.0f, 0.0f);
	cylinder->SetPosition(0.0f, 0.0f, 0.0f);
	donut->SetPosition(4.0f, 0.0f, 0.0f);
	cube->SetPosition(-8.0f, 0.0f, 0.0f);
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

	WireframeControls(dt);
	CursorControls(dt);

	if (enableFlying)
	{
		CameraControls(dt);
	}

	if (GetAsyncKeyState('1')) {
		mCurrentCamera = cameraA;
		graphics->SwitchCamera(cameraA);
	}
	else if (GetAsyncKeyState('2')) {
		mCurrentCamera = cameraB;
		graphics->SwitchCamera(cameraB);
	}

	cameraB->SetPosition(cube->GetPosition());

	XMFLOAT3 cubePos = cube->GetPosition();
	XMFLOAT3 newCameraBPos = XMFLOAT3
	(
		cubePos.x + cameraBOffset.x,
		cubePos.y + cameraBOffset.y,
		cubePos.z + cameraBOffset.z
	);


	float dx = XMConvertToRadians(0.25f * static_cast<float>(graphics->GetMouseX() - mLastMousePosX));
	cameraB->RotateY(dx);

	cameraB->LookAt
	(
		newCameraBPos,
		cubePos
	);

	rotationValue += (rotationSpeed * dt);

	cube->SetRotation(10.0f, rotationValue, 0.0f);
	donut->SetRotation(rotationValue, 0.0f, 0.0f);

	mCurrentCamera->UpdateViewMatrix();

	for (auto& object : mGameObjects) {
		object->Update(dt);
	}
}

void Application::WireframeControls(float dt)
{
	bool changed = false;
	timeSinceSpacePressed += dt;
	if (GetAsyncKeyState(VK_SPACE)) {
		if (timeSinceSpacePressed > spaceTimer) {
			wireframeOn = !wireframeOn;
			changed = true;
			timeSinceSpacePressed = 0.0f;
		}
	}
	if (changed) graphics->EnableWireframe(wireframeOn);
}

void Application::CursorControls(float dt)
{
	timeSinceFPressed += dt;
	if (GetAsyncKeyState('F')) {
		if (timeSinceFPressed > fTimer) {
			timeSinceFPressed = 0.0f;

			enableFlying = !enableFlying;

			clippedCursor = !clippedCursor;
			if (clippedCursor) {
				//graphics->ConfineCursor();
				graphics->HideCursor();
			}
			else
			{
				//graphics->FreeCursor();
				graphics->ShowCursor();
			}
		}
	}
}

void Application::CameraControls(float dt)
{
	if (GetAsyncKeyState('W')) cameraA->Walk(10.0f * dt);
	if (GetAsyncKeyState('S')) cameraA->Walk(-10.0f * dt);
	if (GetAsyncKeyState('A')) cameraA->Strafe(-10.0f * dt);
	if (GetAsyncKeyState('D')) cameraA->Strafe(10.0f * dt);

	float dx = XMConvertToRadians(0.25f * static_cast<float>(graphics->GetMouseX() - mLastMousePosX));
	float dy = XMConvertToRadians(0.25f * static_cast<float>(graphics->GetMouseY() - mLastMousePosY));

	mCurrentCamera->Pitch(dy);
	mCurrentCamera->RotateY(dx);

	mLastMousePosX = graphics->GetMouseX();
	mLastMousePosY = graphics->GetMouseY();
}

void Application::Draw()
{
	graphics->ClearBuffers();

	for (auto& object : mGameObjects) {
		object->Draw();
	}

	groundPlane->Draw();

	graphics->Present();
}