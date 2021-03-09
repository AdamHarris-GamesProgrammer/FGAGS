#include "Level2.h"

void Level2::LoadLevel()
{
	Level::LoadLevel();

	pGroundPlane = new Plane(_pGfx);
	pGroundPlane->Make(20.0f, 20.0f, 8, 8);

	//Loads the texture for the ground plane
	pGroundPlane->CreateTexture(L"Assets/Textures/stone.dds");

	_pGameObjects.push_back(pGroundPlane);
}

void Level2::ExitLevel()
{

}

void Level2::PollInput(float dt)
{

}

void Level2::Update(float dt)
{

}

void Level2::Reset()
{
	delete pGroundPlane;

	LoadLevel();
}

void Level2::DrawUI()
{
	ImGui::Begin("Test 2");

	ImGui::Text("Test 2");


	ResetButton();

	ImGui::End();
}
