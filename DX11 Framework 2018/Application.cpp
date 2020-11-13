#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

Application::Application()
{
}

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

	cameraB = new Camera(
		XMFLOAT3(1.0f, 0.0f, -1.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		graphics->GetWindowWidth(),
		graphics->GetWindowHeight(),
		0.01f, 100.0f
	);

	mCurrentCamera = cameraA;
	graphics->SwitchCamera(cameraA);

	cameraB->SetLookTo(false);

	//Sets deafault positions
	sphere->SetPosition(-4.0f, 0.0f, 0.0f);
	cylinder->SetPosition(0.0f, 0.0f, 0.0f);
	donut->SetRotation(90.0f, 0.0f, 0.0f);
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
	XMFLOAT3 cameraAOffset = XMFLOAT3();


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
		cameraBOffset.z += 1.2f * dt;
		cameraAOffset.z += 1.2f * dt;
	}
	//Zoom Out
	else if (GetAsyncKeyState(VK_NUMPAD5)) {
		cameraBOffset.z -= 1.2f * dt;
		cameraAOffset.z -= 1.2f * dt;
	}

	//Pivot Left
	if (GetAsyncKeyState(VK_NUMPAD4)) {
		cameraBOffset.x -= 1.2f * dt;
		cameraAOffset.x -= 1.2f * dt;
	}

	//Pivot Right
	else if (GetAsyncKeyState(VK_NUMPAD6)) {
		cameraBOffset.x += 1.2f * dt;
		cameraAOffset.x += 1.2f * dt;
	}

	

	rotationValue += (rotationSpeed * dt);

	if (changed) graphics->EnableWireframe(wireframeOn);



	cube->SetRotation(10.0f,rotationValue, 0.0f);
	


	XMFLOAT3 cubePos = cube->GetPosition();

	cameraB->SetAt(cubePos);

	XMFLOAT3 cameraBPos = XMFLOAT3
	(
		cubePos.x + cameraBOffset.x,
		cubePos.y + cameraBOffset.y,
		cubePos.z + cameraBOffset.z
	);

	XMFLOAT3 newCameraAPos = XMFLOAT3
	(
		cameraAPos.x +  cameraAOffset.x,
		cameraAPos.y + cameraAOffset.y,
		cameraAPos.z + cameraAOffset.z
	);

	if (mCurrentCamera == cameraA) {
		mCurrentCamera->SetEye(newCameraAPos);
	}
	else
	{
		mCurrentCamera->SetEye(cameraBPos);
	}

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