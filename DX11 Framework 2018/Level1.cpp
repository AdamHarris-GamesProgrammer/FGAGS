#include "Level1.h"

void Level1::LoadLevel()
{
	Level::LoadLevel();

	_pParticleComponent = new ParticleComponent(_pGameObjects[0]);
	_pParticleComponent->SetAcceleration(Vector3(5.0f, 0.0f, 0.0f));

	LoadGround();
}

void Level1::Update(float dt)
{
}

void Level1::DrawUI()
{
	ImGui::Begin("Test 1");

	//Print Movement Information about our cube.
	OutputPosition(_pGameObjects[0]);
	//OutputVelocity(_pGameObjects[0]);
	//OutputAcceleration(_pGameObjects[0]);
	

	ResetButton();

	ImGui::End();
}
