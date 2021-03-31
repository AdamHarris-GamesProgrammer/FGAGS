#include "Level1.h"

void Level1::LoadLevel()
{
	Level::LoadLevel();

	//_pGameObjects[0]->GetBody()->SetAwake();
	//_pGameObjects[0]->GetBody()->SetAcceleration(10, 0, 0);

	_pRb = new RigidbodyComponent(_pGameObjects[0]);

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

	//Print Movement Information about our cube.
	OutputPosition(_pGameObjects[0]);
	OutputVelocity(_pGameObjects[0]);
	OutputAcceleration(_pGameObjects[0]);
	

	ResetButton();

	ImGui::End();
}
