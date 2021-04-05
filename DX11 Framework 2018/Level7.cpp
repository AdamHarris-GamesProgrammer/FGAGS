#include "Level7.h"

void Level7::LoadLevel()
{
	Level::LoadLevel();

	if (_pParticleComponent == nullptr) {
		_pParticleComponent = std::make_unique<ParticleComponent>(_pGameObjects[0]);
		_pParticleComponent->SetMass(1.0f);
		_pParticleComponent->SetDamping(0.9f);
	}
	else
	{
		_pParticleComponent->SetOwner(_pGameObjects[0]);
	}

	_pGravityForce = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));
	_pBuoyancyForce = std::make_unique<BuoyancyForceGenerator>(7.0f, 1.0f, 7.0f, 1000.0f);
	_pRestingForce = std::make_unique<RestingForceGenerator>(1.0f);

	LoadGround();
}

void Level7::Update(float dt)
{
	_pGravityForce->Update(_pParticleComponent.get(), dt);
	_pBuoyancyForce->Update(_pParticleComponent.get(), dt);
	_pRestingForce->Update(_pParticleComponent.get(), dt);
}

void Level7::DrawUI()
{
	ImGui::Begin("Test 7");

	OutputVector3("Object Position: ", _pGameObjects[0]->GetTransform().GetPosition());

	ResetButton();

	ImGui::End();
}
