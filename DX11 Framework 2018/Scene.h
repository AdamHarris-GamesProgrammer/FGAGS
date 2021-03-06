#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>

#include "Graphics.h"

#include <vector>
#include <algorithm>

#include "RendererComponent.h"

#include "JSONLevelLoader.h"
#include <memory>

using namespace DirectX;

//typedefs to speed up some code for long data types
typedef std::vector<std::shared_ptr<Camera>> Cameras;
typedef std::vector<Object*> GameObjects;

class Scene
{
public:
	//Initializes data for the scene
	Scene(std::shared_ptr<Graphics> gfx, const char* filename);

	//Deallocate memory
	~Scene();

	//Virtual methods for loading a level
	virtual void LoadLevel();
	//Virtual method for exiting a level
	virtual void ExitLevel();
	//Virtual method for polling input in a level
	virtual void PollInput(float dt);

	//Updates the objects in the scene, which also updates their components
	void BeginFrame(float dt);

	//Pure virtual update method for any custom logic in a level
	virtual void Update(float dt) = 0;

	//Renders the current scene
	void EndFrame();

	//Resets our level by reloading it
	void Reset();

protected:
	//Basic Fly Camera implementation for levels to implement if wanted. 
	void FlyCamera(float dt);
	
	//Reset Button For ImGUI
	void ResetButton();
	//Outputs a Vector3 to a ImGUI window
	void OutputVector3(std::string label, Vector3 vec);
	
	//Loads in a ground plane
	void LoadGround();
protected:
	//Shared pointer to our current camera as this needs to be shared with Graphics class
	std::shared_ptr<Camera> _pCurrentCamera;

	//Vectors for Cameras and GameObjects in a scene 
	Cameras _pCameras;
	GameObjects _pGameObjects;
	
	//Pointer to our graphics object
	std::shared_ptr<Graphics> _pGfx = nullptr;

	std::unique_ptr<Object> _pGroundPlane = nullptr;
	std::unique_ptr<RendererComponent> _pGroundPlaneRenderer = nullptr;
private:
	//Abstract method for levels to implement their own UI
	virtual void DrawUI() = 0;

	//Initializes the level loader
	void Initialize();

	//The below methods are used as function pointers in threads, level loading uses multi threading to speed up file reading
	//Loads GameObjects from a file
	void LoadObjectsFromFile(const char* filename);

	//Loads Cameras from a file
	void LoadCameraObjectsFromFile(const char* filename);

	void MakePlane(float width, float depth, UINT m, UINT n, RendererComponent* renderer);
private:
	//Stores the filepath for this level
	const char* _pLevelFile;

	//Stores our LevelLoader object
	std::unique_ptr<JSONLevelLoader> _pLevelLoader;
};


