#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

#include "Graphics.h"
#include "GameObject.h"
#include <vector>

#include "Time.h"

using namespace DirectX;


class Application
{
private:
	Graphics* graphics;

	GameObject sphere;

	D3D_DRIVER_TYPE         _driverType;


	bool wireframeOn = false;
	float timeSinceSpacePressed = 0.0f;
	float spaceTimer = 0.1f;

	float rotationSpeed = 1.0f;

	Time time;
	float rotationValue = 0.0f;
	float previousRotation = 0.0f;

private:

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};

