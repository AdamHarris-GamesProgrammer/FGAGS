#include "Level2.h"

void Level2::LoadLevel()
{
	Level::LoadLevel();

	LoadGround();

	_pGameObjects[0]->GetBody()->SetAwake();

	_pGravityGenerator = new GravityForceGenerator(Vector3(0.0f, -9.81f, 0.0f));
	_pRestingGenerator = new RestingForceGenerator(1.0f);
}

void Level2::ExitLevel()
{

}

void Level2::PollInput(float dt)
{
	Level::PollInput(dt);
}

void Level2::Update(float dt)
{
	_pGravityGenerator->Update(_pGameObjects[0]->GetBody(), dt);
	_pRestingGenerator->Update(_pGameObjects[0]->GetBody(), dt);
}

void Level2::Reset()
{
	LoadLevel();
}

void Level2::DrawUI()
{
	ImGui::Begin("Test 2");

	ImGui::Text("Test 2");


	ResetButton();

	ImGui::End();
}
