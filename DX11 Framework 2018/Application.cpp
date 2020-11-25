#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"


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

	groundPlane = new Plane(graphics);
	groundPlane->Make(25.0f, 25.0f, 8, 8);


	mImGuiManager = new ImGUIManager(graphics);

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
				graphics->HideCursor();
			}
			else
			{
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
	mImGuiManager->BeginFrame();

	{
		ImGui::Begin("Boop");
		ImGui::Text("Hello World");
		ImGui::End();
	}
	

	graphics->ClearBuffers();

	for (auto& object : mGameObjects) {
		object->Draw();
	}

	groundPlane->Draw();


	mImGuiManager->EndFrame();

	graphics->Present();

}

GameObject* Application::FindGameObjectWithName(std::string name)
{
	for (auto& obj : mGameObjects) {
		if (obj->GetName() == name) return obj;
	}
}
