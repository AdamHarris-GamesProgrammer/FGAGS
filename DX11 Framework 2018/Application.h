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
#include "SkySphere.h"

#include <memory>

#include "ParticleForceGenerator.h"
#include "ParticleForceRegistry.h"
#include "ParticleUplift.h"
#include "ParticleAirBrake.h"

#include "ParticleDrag.h"
#include "ParticleSpring.h"
#include "ParticleAnchoredSpring.h"
#include "ParticleBungee.h"
#include "ParticleBuoyancy.h"



#include "Precision.h"
#include "CollisionPrimitives.h"
#include "CollisionData.h"
#include "CollisionDetector.h"
#include "Contacts.h"

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
	void DrawGUI();
	void SelectedObjectControl(float dt);

	void CursorControls(float dt);

	void CameraControls(float dt);

	GameObject* FindGameObjectWithName(std::string name);

private:
	Box* pBottomCube;
	Box* pTopCube;
	CollisionPlane* pGround;


	Contact contacts[MAX_CONTACTS];
	CollisionData cData;
	ContactResolver* cResolver;


	//Time object used to track delta time for object translations
	Time mTime;

	Graphics* pGfx = nullptr;


	//GameObject member variables
	Plane* pGroundPlane = nullptr;

	MeshedObject* pBlendedCube = nullptr;

	GameObject* pSelectedObject = nullptr;

	SkySphere* pSkySphere = nullptr;

	std::vector<GameObject*> pGameObjects;

	//Particle Force Generator Test
	ParticleForceRegistry _registry;

	ParticleDrag* pDrag;


	//Current Camera is stored so only the update code for one camera is called 
	std::shared_ptr<Camera> pCurrentCamera = nullptr;

	std::vector<std::shared_ptr<Camera>> pCameras;

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
	Vector3 mCameraBOffset = Vector3(0.0f, 4.5f, -10.0f);

	//Boolean member variables
	bool mWireframeEnabled = false;
	bool mFlyingEnabled = false;
	bool mCursorClipped = false;
};

