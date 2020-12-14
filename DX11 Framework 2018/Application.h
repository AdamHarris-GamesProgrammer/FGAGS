#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

#include "Graphics.h"

#include <vector>

#include "ImGUIManager.h"
#include "JSONLevelLoader.h"

#include "Time.h"
#include "Camera.h"
#include "Plane.h"

#include <memory>

using namespace DirectX;


class Application
{
public:
	Application() = default;
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();

	void Draw();

	

private:
	void Picking();
	void PollInput(float dt);
	void DrawGUI();
	void SelectedObjectControl(float dt);

	void CursorControls(float dt);

	void CameraControls(float dt);

	GameObject* FindGameObjectWithName(std::string name);

private:
	//Time object used to track delta time for object translations
	Time mTime;

	Graphics* pGfx = nullptr;


	//GameObject member variables
	GameObject* pCube = nullptr;
	GameObject* pDonut = nullptr;
	Plane* pGroundPlane = nullptr;

	MeshedObject* pBlendedCube = nullptr;

	MeshedObject* pSkySphere = nullptr;
	GameObject* pSelectedObject = nullptr;

	std::vector<GameObject*> pGameObjects;

	//Camera member variables
	std::shared_ptr<Camera> pCameraA = nullptr;
	std::shared_ptr<Camera> pCameraB = nullptr;
	std::shared_ptr<Camera> pCameraC = nullptr;
	std::shared_ptr<Camera> pCameraD = nullptr;

	//Current Camera is stored so only the update code for one camera is called 
	std::shared_ptr<Camera> pCurrentCamera = nullptr;

	//ImGuiManager object
	ImGUIManager mImGuiManager;

	//Level loader object 
	JSONLevelLoader mJSONLevelLoader;

	//Sets the default clear colour to a sky blue
	float mClearColor[4] = { 0.583f, 0.639f, 0.743f, 1.0f };

	//Key timing member variables
	//Timers used to stop the code from rapid firing when key is pressed
	float mTimeSinceGPressed = 0.0f;
	float mGKeyTimer = 0.1f;

	//Object movement and rotation related member variables
	float mObjectRotationValue = 0.0f;
	float mObjectRotationSpeed = 1.0f;
	float mObjectMovementSpeed = 5.0f;

	//Offset variable for positioning camera B around a object
	XMFLOAT3 mCameraBOffset = XMFLOAT3(0.0f, 4.5f, -10.0f);

	//Boolean member variables
	bool mWireframeEnabled = false;
	bool mFlyingEnabled = false;
	bool mCursorClipped = false;
};

