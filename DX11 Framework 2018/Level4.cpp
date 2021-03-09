#include "Level4.h"

void Level4::LoadLevel()
{
	Level::LoadLevel();

	LoadGround();
}

void Level4::ExitLevel()
{

}

void Level4::PollInput(float dt)
{

}

void Level4::Update(float dt)
{

}

void Level4::Reset()
{
	LoadLevel();
}

void Level4::DrawUI()
{
	ImGui::Begin("Test 4");

	ImGui::Text("Test 4");

	ResetButton();

	ImGui::End();
}
