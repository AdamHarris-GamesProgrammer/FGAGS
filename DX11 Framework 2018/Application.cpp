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

	sphere.SetPosition(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	sphere.SetRotation(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));
	sphere.SetScale(XMFLOAT4(1.0f,1.0f,1.0f, 0.0f));

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

	sphere.Update(time.DeltaTime());


	//cubes.resize(3);

	//Sun
	XMMATRIX plane = XMMatrixIdentity();
	plane = XMMatrixMultiply(plane, XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(-0.2,-0.2,0) * XMMatrixRotationRollPitchYaw(rotationValue, rotationValue, 0));
	sphere.SetTransform(plane);


	////Earth
	//XMMATRIX earth = XMMatrixIdentity();
	//earth = XMMatrixMultiply(earth, XMMatrixRotationRollPitchYaw(0,t * 2,0) * XMMatrixScaling(.25,.25,.25) *
	//XMMatrixTranslation(6,0,0) * XMMatrixRotationRollPitchYaw(0, t * 2, 0));
	//XMStoreFloat4x4(&cubes[1], earth);


	////Earth moon
	//XMMATRIX earthMoon = XMMatrixIdentity();
	//earthMoon = XMMatrixMultiply(earthMoon, XMMatrixScaling(.1, .1, .1) * XMMatrixTranslation(3, 0, 0) *
	//	XMMatrixRotationRollPitchYaw(0, t * 1,0) * XMMatrixTranslation(6, 0, 0) * XMMatrixRotationRollPitchYaw(0, t * 2, 0));
	//XMStoreFloat4x4(&cubes[2], earthMoon);
}

void Application::Draw()
{
	graphics->ClearBuffers();
	graphics->SetShaders();

	graphics->UpdateBuffers(sphere.GetTransform(), 0);
	sphere.Draw(graphics);

	graphics->Present();
}