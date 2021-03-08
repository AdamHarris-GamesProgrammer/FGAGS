#include "Level.h"
#include <thread>

void Level::LoadLevel()
{
	_pGameObjects.clear();

	std::thread objectLoader(&Level::LoadObjectsFromFile, this, _pLevelFile);
	std::thread cameraLoader(&Level::LoadCameraObjectsFromFile, this, _pLevelFile);

	cameraLoader.join();
	_pCurrentCamera = _pCameras[0];
	_pGfx->SetCurrentCamera(_pCurrentCamera);

	objectLoader.join();
}

void Level::BeginUpdate(float dt)
{
	_pCurrentCamera->Update(dt);

	for (auto& object : _pGameObjects) {
		object->Update(dt);
	}
}

void Level::EndUpdate(float dt)
{
	for (auto& object : _pGameObjects) {
		object->UpdateTransforms();
	}
}

void Level::Render()
{
	_pGfx->BeginFrame();

	for (auto& object : _pGameObjects) {
		object->Draw();
	}

	DrawUI();

	_pGfx->EndFrame();
}

void Level::FlyCamera(float dt)
{
	if (GetAsyncKeyState('W')) _pCameras[0]->Walk(10.0f * dt);
	else if (GetAsyncKeyState('S')) _pCameras[0]->Walk(-10.0f * dt);
	if (GetAsyncKeyState('A')) _pCameras[0]->Strafe(-10.0f * dt);
	else if (GetAsyncKeyState('D')) _pCameras[0]->Strafe(10.0f * dt);

	if (GetAsyncKeyState('Q')) _pCameras[0]->RotateY(-1.0f * dt);
	else if (GetAsyncKeyState('E')) _pCameras[0]->RotateY(1.0f * dt);

	if (GetAsyncKeyState('R')) _pCameras[0]->Pitch(-1.0f * dt);
	else if (GetAsyncKeyState('F')) _pCameras[0]->Pitch(1.0f * dt);
}

void Level::DrawUI()
{

}

void Level::Initialize()
{
	_levelLoader = JSONLevelLoader(_pGfx);
}

void Level::LoadObjectsFromFile(const char* filename)
{
	_pGameObjects = _levelLoader.LoadObjectsFromFile(filename);
}

void Level::LoadCameraObjectsFromFile(const char* filename)
{
	_pCameras = _levelLoader.LoadCamerasFromFile(filename);
}
