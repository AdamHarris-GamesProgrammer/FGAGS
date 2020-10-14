#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

#include "Graphics.h"
#include <vector>

#include "Time.h"

using namespace DirectX;





class Application
{
private:
	Graphics* graphics;


	D3D_DRIVER_TYPE         _driverType;


	XMFLOAT4X4              _view;
	XMFLOAT4X4              _projection;

	std::vector<XMFLOAT4X4> cubes;

	//bool wireframeOn = false;
	//float timeSinceSpacePressed = 0.0f;
	//float spaceTimer = 0.1f;

	float rotationSpeed = 150.0f;

	Time time;

private:

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};

