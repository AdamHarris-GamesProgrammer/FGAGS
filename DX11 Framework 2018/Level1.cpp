#include "Level1.h"

void Level1::LoadLevel()
{
	Level::LoadLevel();

	_pGameObjects[0]->GetBody()->SetAwake();
	_pGameObjects[0]->GetBody()->SetAcceleration(10, 0, 0);

	LoadGround();
}

void Level1::PollInput(float dt)
{
	Level::PollInput(dt);
}

void Level1::Update(float dt)
{



}

void Level1::Reset()
{
	LoadLevel();
}

void Level1::DrawUI()
{
	ImGui::Begin("Test 1");

	Vector3 objPos = _pGameObjects[0]->GetTransform().GetPosition();
	Vector3 objVel = _pGameObjects[0]->GetBody()->GetVelocity();
	Vector3 objAcc = _pGameObjects[0]->GetBody()->GetAcceleration();

	ImGui::Text("Object Position: {X: %f, Y: %f, Z: %f}", objPos.x, objPos.y, objPos.z);
	ImGui::Text("Object Velocity: {X: %f, Y: %f, Z: %f}", objVel.x, objVel.y, objVel.z);
	ImGui::Text("Object Acceleration: {X: %f, Y: %f, Z: %f}", objAcc.x, objAcc.y, objAcc.z);

	ResetButton();

	ImGui::End();
}
