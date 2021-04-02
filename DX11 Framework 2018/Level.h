#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

#include "Graphics.h"

#include <vector>
#include <algorithm>

#include "GameObject.h"
#include "JSONLevelLoader.h"
#include <memory>

#include "CollisionPrimitives.h"
#include "CollisionData.h"
#include "CollisionDetector.h"
#include "Contact.h"
#include "ContactResolver.h"

#include "Plane.h"

using namespace DirectX;

typedef std::vector<std::shared_ptr<Camera>> Cameras;
typedef std::vector<GameObject*> GameObjects;

class Level
{
public:
	Level(Graphics* gfx, const char* filename) : _pGfx(gfx), _pLevelFile(filename) 
	{
		Initialize();
	}

	~Level() {
		delete _pLevelFile;
	}

	virtual void LoadLevel();
	virtual void ExitLevel();
	virtual void PollInput(float dt);

	void BeginUpdate(float dt);
	virtual void Update(float dt) = 0;

	void Render();

	virtual void Reset() {
		LoadLevel();
	}

protected:
	void FlyCamera(float dt);
	
	
	void ResetButton();
	void OutputVector3(std::string label, Vector3 vec);
	

	void LoadGround();
protected:
	std::shared_ptr<Camera> _pCurrentCamera;
	Cameras _pCameras;
	GameObjects _pGameObjects;

	Graphics* _pGfx = nullptr;

	Plane* _pGroundPlane = nullptr;
private:
	virtual void DrawUI() = 0;

	

	void Initialize();

	void LoadObjectsFromFile(const char* filename);
	void LoadCameraObjectsFromFile(const char* filename);
private:

	const char* _pLevelFile;


	JSONLevelLoader _levelLoader;
};


