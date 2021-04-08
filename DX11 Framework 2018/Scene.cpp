#include "Scene.h"
#include <thread>
#include <sstream>

Scene::Scene(Graphics* gfx, const char* filename) : _pGfx(gfx), _pLevelFile(filename)
{
	Initialize();
}

Scene::~Scene()
{
	delete _pLevelFile;
	_pLevelFile = nullptr;

	//Application class handles deleting the Graphics object pointer
	_pGfx = nullptr;

	//Deletes our ground plane
	delete _pGroundPlane;
	_pGroundPlane = nullptr;

	//Clears the Camera and Object vectors
	_pCameras.clear();
	_pGameObjects.clear();
}

void Scene::LoadLevel()
{
	//Clears the memory associated with our GameObject and Camera vectors
	_pGameObjects.clear();
	_pCameras.clear();

	//Starts up two threads, one for object loading and one for camera loading
	std::thread objectLoader(&Scene::LoadObjectsFromFile, this, _pLevelFile);
	std::thread cameraLoader(&Scene::LoadCameraObjectsFromFile, this, _pLevelFile);

	//Waits for the camera thread to be done
	cameraLoader.join();
	_pCurrentCamera = _pCameras[0];
	_pGfx->SetCurrentCamera(_pCurrentCamera);

	//Waits for the object thread to be done
	objectLoader.join();
}

void Scene::ExitLevel()
{
	_pGameObjects.clear();
	_pCameras.clear();
}

void Scene::PollInput(float dt)
{
	//Resets the scene
	if (GetAsyncKeyState('R')) {
		Reset();
	}
}

void Scene::BeginUpdate(float dt)
{
	//Begins our frame (mainly ImGUI prep)
	_pGfx->BeginFrame();

	//Updates the scene camera
	_pCurrentCamera->Update(dt);

	//Updates the objects in the 
	for (auto& object : _pGameObjects) {
		object->Update(dt);
	}

	if (_GroundPlane != nullptr) {
		_GroundPlane->Update(dt);
	}
}

void Scene::Render()
{
	
	////Draws each object
	//for (auto& object : _pGameObjects) {
	//	object->Draw();
	//}

	//Draws the level UI
	DrawUI();

	//Ends our frame (swaps framebuffers etc)
	_pGfx->EndFrame();
}

void Scene::Reset()
{
	LoadLevel();
}

void Scene::FlyCamera(float dt)
{
	//Move forward
	if (GetAsyncKeyState('W')) _pCameras[0]->Walk(10.0f * dt);
	//Move backward
	else if (GetAsyncKeyState('S')) _pCameras[0]->Walk(-10.0f * dt);
	//Move left
	if (GetAsyncKeyState('A')) _pCameras[0]->Strafe(-10.0f * dt);
	//Move right
	else if (GetAsyncKeyState('D')) _pCameras[0]->Strafe(10.0f * dt);

	//Rotate left
	if (GetAsyncKeyState('Q')) _pCameras[0]->RotateY(-1.0f * dt);
	//Rotate right
	else if (GetAsyncKeyState('E')) _pCameras[0]->RotateY(1.0f * dt);
	//Rotate upwards
	if (GetAsyncKeyState('R')) _pCameras[0]->Pitch(-1.0f * dt);
	//Rotate downwards
	else if (GetAsyncKeyState('F')) _pCameras[0]->Pitch(1.0f * dt);
}

void Scene::ResetButton()
{
	if (ImGui::Button("Reset Simulation")) {
		Reset();
	}
}

void Scene::OutputVector3(std::string label, Vector3 vec)
{
	//Creates a ostringstream object for simplifying outputting the vector
	std::ostringstream output;
	//Sets it to six significant figures, for the sakes of this project this is fine. However after 9999.99 it will go down to 1dp
	output.precision(6);
	//Prepares the ostringstream
	output << label << " {X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z << "}";
	//Converts the output to a const char* for ImGUI
	ImGui::Text(output.str().c_str());
}

void Scene::LoadGround()
{
	//Deletes memory associated with the old ground plane
	if (_pGroundPlane != nullptr) delete _pGroundPlane;

	//Creates a new ground plane object
	_pGroundPlane = new Plane();

	_GroundPlane = new Object();
	_GroundPlaneRenderer = new RendererComponent(_GroundPlane, _pGfx);

	_pGroundPlane->Make(20.f, 20.0f, 7, 7, _GroundPlaneRenderer);

	_GroundPlaneRenderer->CreateTexture(L"Assets/Textures/stone.dds");

	//_pGameObjects.push_back(_GroundPlane);

}

void Scene::Initialize()
{
	//Create memory for the level loader
	_pLevelLoader = std::make_unique<JSONLevelLoader>(_pGfx);
}

void Scene::LoadObjectsFromFile(const char* filename)
{
	_pGameObjects = _pLevelLoader->LoadObjectsFromFile(filename);
}

void Scene::LoadCameraObjectsFromFile(const char* filename)
{
	_pCameras = _pLevelLoader->LoadCamerasFromFile(filename);
}
