#include "Application.h"
#include <iostream>
#include <DirectXMath.h>

Application::Application()
{
}

Application::~Application()
{
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	graphics = new Graphics();
	graphics->Initialise(hInstance, nCmdShow);
	
	sphere = GameObject(graphics, "Assets/Models/Sphere.obj");

	cube = Cube(graphics);

;

	return S_OK;
}

void Application::Update()
{
	time.Tick();

	static float currentTime = GetTickCount();

	// Update our rotation values
	static float t = 0.0f;

	if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();

		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;

		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	bool changed = false;
	timeSinceSpacePressed += time.DeltaTime();
	if (GetAsyncKeyState(VK_SPACE)) {
		if (timeSinceSpacePressed > spaceTimer) {
			wireframeOn = !wireframeOn;
			changed = true;
			timeSinceSpacePressed = 0.0f;
		}
	}


	rotationValue += rotationSpeed;
	previousRotation = rotationValue;

	if (changed) graphics->EnableWireframe(wireframeOn);

	sphere.SetRotation(XMFLOAT4(0.0f, rotationValue * time.DeltaTime(), 0.0f, 0.0f));


	sphere.Update(t);
	cube.Update(t);
}

void Application::Draw()
{
	graphics->ClearBuffers();
	graphics->SetShaders();

	sphere.Draw();

	cube.Draw();

	graphics->Present();
}