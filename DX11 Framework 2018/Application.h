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
#include "Level6.h"
#include "Level7.h"

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

private:
	//Stores the current level we are in so we only draw and update this scene
	Level* _pCurrentLevel = nullptr;
	//Stores all of our levels we have
	Level1* _pLevel1 = nullptr;
	Level2* _pLevel2 = nullptr;
	Level3* _pLevel3 = nullptr;
	Level4* _pLevel4 = nullptr;
	Level5* _pLevel5 = nullptr;
	Level6* _pLevel6 = nullptr;
	Level7* _pLevel7 = nullptr;
	

	//Time object used to track delta time for object translations
	Time _time;

	Graphics* _pGfx = nullptr;


	//Sets the default clear colour to a sky blue
	float _clearColor[4] = { 0.583f, 0.639f, 0.743f, 1.0f };
};

