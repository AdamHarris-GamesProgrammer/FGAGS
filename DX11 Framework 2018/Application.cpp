#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

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


	cameraA = new Camera();
	cameraA->SetLens(45.0f, graphics->GetWindowWidth() / graphics->GetWindowHeight(), 0.01f, 100.0f);
	mCurrentCamera = cameraA;
	graphics->SwitchCamera(cameraA);

	graphics->ConfineCursor();

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

	WireframeControls(dt);
	CursorControls(dt);
	CameraControls(dt);


	cube->SetRotation(10.0f, rotationValue, 0.0f);
	donut->SetRotation(rotationValue, 0.0f, 0.0f);


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
			clippedCursor = !clippedCursor;
			if (clippedCursor) {
				graphics->ConfineCursor();
			}
			else
			{
				graphics->FreeCursor();
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

	mCurrentCamera->UpdateViewMatrix();

	rotationValue += (rotationSpeed * dt);
}

void Application::Draw()
{
	graphics->ClearBuffers();

	for (auto& object : mGameObjects) {
		object->Draw();
	}

	graphics->Present();
}