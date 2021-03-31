#include "Level.h"
#include <thread>

void Level::LoadLevel()
{
	_pGameObjects.clear();
	_pCameras.clear();

	std::thread objectLoader(&Level::LoadObjectsFromFile, this, _pLevelFile);
	std::thread cameraLoader(&Level::LoadCameraObjectsFromFile, this, _pLevelFile);

	cameraLoader.join();
	_pCurrentCamera = _pCameras[0];
	_pGfx->SetCurrentCamera(_pCurrentCamera);

	objectLoader.join();
}

void Level::ExitLevel()
{

}

void Level::PollInput(float dt)
{
	if (GetAsyncKeyState('R')) {
		Reset();
	}
}

void Level::BeginUpdate(float dt)
{
	_pCurrentCamera->Update(dt);

	for (auto& object : _pGameObjects) {
		object->Update(dt);
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

void Level::ResetButton()
{
	if (ImGui::Button("Reset Simulation")) {
		Reset();
	}
}

void Level::OutputVelocity(GameObject* obj)
{
	Vector3 objVel = dynamic_cast<RigidbodyComponent*>(obj->GetComponent(Rigidbody))->GetVelocity();
	ImGui::Text("Object Velocity: {X: %.2f, Y: %.2f, Z: %.2f}", objVel.x, objVel.y, objVel.z);
}

void Level::OutputPosition(GameObject* obj)
{
	Vector3 objPos = obj->GetTransform()->GetPosition();
	ImGui::Text("Object Position: {X: %.2f, Y: %.2f, Z: %.2f}", objPos.x, objPos.y, objPos.z);
}

void Level::OutputAcceleration(GameObject* obj)
{
	Vector3 objAcc = dynamic_cast<RigidbodyComponent*>(obj->GetComponent(Rigidbody))->GetAcceleration();
	ImGui::Text("Object Acceleration: {X: %.2f, Y: %.2f, Z: %.2f}", objAcc.x, objAcc.y, objAcc.z);
}

void Level::LoadGround()
{
	if (_pGroundPlane != nullptr) delete _pGroundPlane;

	_pGroundPlane = new Plane(_pGfx);
	_pGroundPlane->Make(20.f, 20.0f, 7, 7);

	_pGroundPlane->CreateTexture(L"Assets/Textures/stone.dds");

	_pGameObjects.push_back(_pGroundPlane);
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
