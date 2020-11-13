#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

#include "Graphics.h"

#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Donut.h"

#include <vector>

#include "Time.h"
#include "FollowCamera.h"

using namespace DirectX;


class Application
{
private:
	Graphics* graphics;

	Sphere* sphere;
	Cube* cube;
	Cylinder* cylinder;
	Donut* donut;

	std::vector<GameObject*> mGameObjects;


	Camera* cameraA = nullptr;
	FollowCamera* cameraB = nullptr;
	Camera* mCurrentCamera = nullptr;

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

