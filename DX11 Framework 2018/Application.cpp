#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

Application::Application() {}

Application::~Application()
{
	delete cameraA;
	delete cameraB;

	cameraA = nullptr;
	cameraB = nullptr;
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	graphics = new Graphics();
	graphics->Initialise(hInstance, nCmdShow);
	
	sphere = new Sphere(graphics);
	cube = new Cube(graphics);
	cylinder = new Cylinder(graphics);
	donut = new Donut(graphics);

	mGameObjects.push_back(sphere);
	mGameObjects.push_back(cube);
	mGameObjects.push_back(cylinder);
	mGameObjects.push_back(donut);

	sphere->CreateTexture(L"Assets/Textures/Crate_COLOR.dds");
	sphere->CreateTexture(L"Assets/Textures/Crate_SPEC.dds");

	cube->CreateTexture(L"Assets/Textures/Marble_COLOR.dds");
	cube->CreateTexture(L"Assets/Textures/Marble_SPEC.dds");

	cylinder->CreateTexture(L"Assets/Textures/Crate_Color.dds");
	donut->CreateTexture(L"Assets/Textures/Marble_COLOR.dds");
	
	cameraA = new Camera(
		XMFLOAT3(3.0f, 0.0f, -10.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		graphics->GetWindowWidth(),
		graphics->GetWindowHeight(), 
		0.01f, 100.0f
	);

	cameraB = new FollowCamera(
		cube,
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		graphics->GetWindowWidth(),
		graphics->GetWindowHeight(),
		0.01f, 100.0f
	);

	cameraB->SetOffset(XMFLOAT3(3.0f, 1.0f, -4.0f));

	mCurrentCamera = cameraA;
	graphics->SwitchCamera(cameraA);

	//Sets default positions
	sphere->SetPosition(-4.0f, 0.0f, 0.0f);
	cylinder->SetPosition(0.0f, 0.0f, 0.0f);
	donut->SetPosition(4.0f, 0.0f, 0.0f);
	cube->SetPosition(-8.0f, 0.0f, 0.0f);

	rotationValue = 0.0f;

	time = Time();

	time.Reset();

	return S_OK;
}

void Application::Update()
{
	time.Tick();
	float dt = time.DeltaTime();

	bool changed = false;
	timeSinceSpacePressed += dt;
	if (GetAsyncKeyState(VK_SPACE)) {
		if (timeSinceSpacePressed > spaceTimer) {
			wireframeOn = !wireframeOn;
			changed = true;
			timeSinceSpacePressed = 0.0f;
		}
	}

	XMFLOAT3 cameraAPos = cameraA->GetEye();

	//Camera A - Stationary
	if (GetAsyncKeyState(VK_NUMPAD1)) {
		mCurrentCamera = cameraA;
		graphics->SwitchCamera(cameraA);
	}
	//Camera B - Follow Camera
	else if (GetAsyncKeyState(VK_NUMPAD2)) {
		graphics->SwitchCamera(cameraB);
		mCurrentCamera = cameraB;
	}

	//Zoom In
	if (GetAsyncKeyState(VK_NUMPAD8)) {
		cameraAPos.z += 1.2f * dt;
	}
	//Zoom Out
	else if (GetAsyncKeyState(VK_NUMPAD5)) {
		cameraAPos.z -= 1.2f * dt;
	}

	//Pivot Left
	if (GetAsyncKeyState(VK_NUMPAD4)) {
		cameraAPos.x -= 1.2f * dt;
	}

	//Pivot Right
	else if (GetAsyncKeyState(VK_NUMPAD6)) {
		cameraAPos.x += 1.2f * dt;
	}

	rotationValue += (rotationSpeed * dt);

	if (changed) graphics->EnableWireframe(wireframeOn);



	cube->SetRotation(10.0f,rotationValue, 0.0f);
	donut->SetRotation(rotationValue, 0.0f, 0.0f);

	if (mCurrentCamera == cameraA) {
		mCurrentCamera->SetEye(cameraAPos);
	}

	mCurrentCamera->PollInput(dt);

	for (auto& object : mGameObjects) {
		object->Update(dt);
	}

	graphics->UpdateCamera();
}

void Application::Draw()
{
	graphics->ClearBuffers();
	graphics->SetShaders();

	for (auto& object : mGameObjects) {
		object->Draw();
	}

	graphics->Present();
}