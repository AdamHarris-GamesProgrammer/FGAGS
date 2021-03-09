#include "Level3.h"

void Level3::LoadLevel()
{
	Level::LoadLevel();

	LoadGround();
}

void Level3::ExitLevel()
{
}

void Level3::PollInput(float dt)
{
}

void Level3::Update(float dt)
{
}

void Level3::Reset()
{
	LoadLevel();

}

void Level3::DrawUI()
{
	ImGui::Begin("Test 3");

	ImGui::Text("Test 3");

	ResetButton();

	ImGui::End();
}
