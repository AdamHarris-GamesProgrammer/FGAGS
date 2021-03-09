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
#include "TerrainPlane.h"
#include "SkySphere.h"

#include <memory>

#include "CollisionPrimitives.h"
#include "CollisionData.h"
#include "CollisionDetector.h"
#include "Contact.h"
#include "ContactResolver.h"

#include "Level.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"

using namespace DirectX;

#define MAX_CONTACTS 10

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

	void ChangeLevel(Level* newLevel);


	void DrawGUI();

	void CursorControls(float dt);

private:
	Level* _pCurrentLevel = nullptr;
	Level1* _pLevel1 = nullptr;
	Level2* _pLevel2 = nullptr;
	Level3* _pLevel3 = nullptr;
	Level4* _pLevel4 = nullptr;
	Level5* _pLevel5 = nullptr;
	


	//Time object used to track delta time for object translations
	Time mTime;

	Graphics* pGfx = nullptr;


	//Sets the default clear colour to a sky blue
	float mClearColor[4] = { 0.583f, 0.639f, 0.743f, 1.0f };

	//Key timing member variables
	//Timers used to stop the code from rapid firing when key is pressed
	float mTimeSinceGPressed = 0.0f;
	float mGKeyTimer = 0.1f;

	//Boolean member variables
	bool mWireframeEnabled = false;
	bool mFlyingEnabled = false;
	bool mCursorClipped = false;

	unsigned int currentTest = 1;
};

