#include "Level7.h"

void Level7::LoadLevel()
{
	Level::LoadLevel();

	if(_pParticleAnchor == nullptr){
		_pParticleAnchor = std::make_unique<ParticleComponent>(_pGameObjects[0]);
		_pParticleAnchor->SetAwake();
		_pParticleAnchor->SetCanSleep(false);
		_pParticleAnchor->SetInverseMass(3.0f);
		_pParticleAnchor->SetDamping(0.9f);
	}
	else
	{
		_pParticleAnchor->SetOwner(_pGameObjects[0]);
	}


	if (_pParticleA == nullptr) {
		_pParticleA = std::make_unique<ParticleComponent>(_pGameObjects[1]);
		_pParticleA->SetAwake();
		_pParticleA->SetCanSleep(false);
		_pParticleA->SetInverseMass(3.0f);
		_pParticleA->SetDamping(0.9f);
	}
	else {
		_pParticleA->SetOwner(_pGameObjects[1]);
	}

	if (_pParticleBungee == nullptr) {
		_pParticleBungee = std::make_unique<ParticleBungee>(_pParticleAnchor.get(), 0.3f, 0.1f);
	}

	LoadGround();
}

void Level7::Update(float dt)
{
	_pParticleBungee->Update(_pParticleA.get(), dt);
}

void Level7::DrawUI()
{
	ImGui::Begin("Test 7");

	float rl = _pParticleBungee->GetRestLength();
	float sc = _pParticleBungee->GetSpringConstant();

	ImGui::Text("Bungee Settings");
	ImGui::SliderFloat("Rest Length: ", &rl, 0.0f, 10.0f, "%.2f", 1.0f);
	ImGui::SliderFloat("Spring Constant: ", &sc, 0.0f, 10.0f, "%.2f", 1.0f);

	_pParticleBungee->SetRestLength(rl);
	_pParticleBungee->SetSpringConstant(sc);


	ResetButton();


	ImGui::End();
}
