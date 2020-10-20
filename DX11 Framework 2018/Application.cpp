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

	//graphics->MakeTrianglePlane(5, 5);

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

	rotationValue = t;

	if (changed) graphics->EnableWireframe(wireframeOn);



	cubes.resize(5);

	//Sun
	XMMATRIX sun = XMMatrixIdentity();
	sun = XMMatrixMultiply(sun, XMMatrixScaling(2, 2, 2) * XMMatrixTranslation(0, 0, 0) * XMMatrixRotationRollPitchYaw(90, 0, 0));
	XMStoreFloat4x4(&cubes[0], sun);

	//Earth
	XMMATRIX earth = XMMatrixIdentity();
	earth = XMMatrixMultiply(earth, XMMatrixRotationRollPitchYaw(0,t * 2,0) * XMMatrixScaling(.25,.25,.25) *
	XMMatrixTranslation(9,0,0) * XMMatrixRotationRollPitchYaw(0, t * 2, 0));
	XMStoreFloat4x4(&cubes[1], earth);


	//Earth moon
	XMMATRIX earthMoon = XMMatrixIdentity();
	earthMoon = XMMatrixMultiply(earthMoon, XMMatrixScaling(.125, .125, .125) * XMMatrixTranslation(3, 0, 0) *
		XMMatrixRotationRollPitchYaw(0, t * 1,0) * XMMatrixTranslation(9, 0, 0) * XMMatrixRotationRollPitchYaw(0, t * 2, 0));
	XMStoreFloat4x4(&cubes[2], earthMoon);


	//Planet 1
	//XMStoreFloat4x4(&_world2, XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(t * 4.0f) * XMMatrixTranslation(-1.5f, 0.0f, 0.0f) * XMMatrixRotationY(t));
	//XMStoreFloat4x4(&_world4, XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(t * 7.0f) * XMMatrixTranslation(-1.0f, 0.0f, 0.0f) * XMMatrixRotationY(t * 3.0f) * XMMatrixTranslation(-2.0f, 0.0f, 0.0f) * XMMatrixRotationY(t));
}

void Application::Draw()
{
	graphics->ClearBuffers();
	graphics->SetShaders();
	/*for (auto& object : cubes)
	{
		graphics->UpdateBuffers(object, 0);
		graphics->Draw(96);
	}*/

	graphics->UpdateBuffers(cubes[0], 0);
	graphics->Draw(96);

	graphics->Present();
}