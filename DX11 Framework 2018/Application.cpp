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
	
	sphere = Sphere(graphics);
	cube = Cube(graphics);
	cylinder = Cylinder(graphics);
	donut = Donut(graphics);

	sphere.CreateTexture(L"Assets/Textures/Crate_COLOR.dds");
	sphere.CreateTexture(L"Assets/Textures/Crate_SPEC.dds");

	cube.CreateTexture(L"Assets/Textures/Marble_COLOR.dds");
	cube.CreateTexture(L"Assets/Textures/Marble_SPEC.dds");

	cylinder.CreateTexture(L"Assets/Textures/Crate_Color.dds");
	donut.CreateTexture(L"Assets/Textures/Marble_COLOR.dds");
	
	cameraA = new Camera(
		XMFLOAT3(3.0f, 0.0f, -10.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		graphics->GetWindowWidth(),
		graphics->GetWindowHeight(), 
		0.01f, 100.0f
	);

	cameraB = new Camera(
		XMFLOAT3(0.0f, 0.0f, -10.0f),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		graphics->GetWindowWidth(),
		graphics->GetWindowHeight(),
		0.01f, 100.0f
	);

	graphics->SwitchCamera(cameraA);

	rotationValue = 0.0f;

	time = Time();

	time.Reset();

	return S_OK;
}

void Application::Update()
{
	time.Tick();

	bool changed = false;
	timeSinceSpacePressed += time.DeltaTime();
	if (GetAsyncKeyState(VK_SPACE)) {
		if (timeSinceSpacePressed > spaceTimer) {
			wireframeOn = !wireframeOn;
			changed = true;
			timeSinceSpacePressed = 0.0f;
		}
	}

	if (GetAsyncKeyState(VK_NUMPAD1)) {
		graphics->SwitchCamera(cameraA);
	}
	else if (GetAsyncKeyState(VK_NUMPAD2)) {
		graphics->SwitchCamera(cameraB);
	}

	float dt = time.DeltaTime();

	rotationValue += (rotationSpeed * dt);

	if (changed) graphics->EnableWireframe(wireframeOn);

	graphics->UpdateCamera();

	cube.SetPosition(-8.0f, 0.0f, 0.0f);
	cube.SetScale(1.0f, 1.0f, 1.0f);
	cube.SetRotation(10.0f,rotationValue, 0.0f);
	sphere.SetPosition(-4.0f, 0.0f, 0.0f);
	cylinder.SetPosition(0.0f, 0.0f, 0.0f);
	donut.SetRotation(90.0f, 0.0f, 0.0f);
	donut.SetPosition(4.0f, 0.0f, 0.0f);


	sphere.Update(dt);
	cube.Update(dt);
	cylinder.Update(dt);
	donut.Update(dt);
}

void Application::Draw()
{
	graphics->ClearBuffers();
	graphics->SetShaders();

	sphere.Draw();
	cube.Draw();
	cylinder.Draw();
	donut.Draw();

	graphics->Present();
}