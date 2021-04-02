#include "Level1.h"

void Level1::LoadLevel()
{
	Level::LoadLevel();

	_pParticleComponent = new ParticleComponent(_pGameObjects[0]);
	_pParticleComponent->SetAcceleration(Vector3(5.0f, 0.0f, 0.0f));
	_pParticleComponent->SetInverseMass(3.0f);
	_pParticleComponent->SetDamping(0.9f);

	LoadGround();
}

void Level1::Update(float dt)
{

}

void Level1::DrawUI()
{
	ImGui::Begin("Test 1");

	//Print Movement Information about our cube.
	OutputVector3("Object Position: ", _pGameObjects[0]->GetTransform().GetPosition());
	OutputVector3("Object Velocity: ", _pParticleComponent->GetVelocity());
	OutputVector3("Object Acceleration", _pParticleComponent->GetAcceleration());

	ResetButton();

	ImGui::End();
}
