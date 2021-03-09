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

	ImGui::Text("Test 1");

	ResetButton();

	ImGui::End();
}
