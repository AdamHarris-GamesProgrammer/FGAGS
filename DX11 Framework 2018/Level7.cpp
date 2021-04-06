#include "Level7.h"

void Level7::LoadLevel()
{
	Level::LoadLevel();

	if (_pParticleComponent == nullptr) {
		_pParticleComponent = std::make_unique<ParticleComponent>(_pGameObjects[0]);
		_pParticleComponent->SetMass(5.0f);
		_pParticleComponent->SetDamping(0.9f);
	}
	else
	{
		_pParticleComponent->SetOwner(_pGameObjects[0]);
	}

	if (_pGravityForce == nullptr) {
		_pGravityForce = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));
	}

	if (_pBuoyancyForce == nullptr) {
		_pBuoyancyForce = std::make_unique<BuoyancyForceGenerator>(7.0f, 5.0f, 7.0f, 1000.0f);
	}

	_currentWaveHeight = _pGameObjects[1]->GetTransform().GetPosition().y;

	_timer = 0.0f;
}

void Level7::Update(float dt)
{
	_pGravityForce->Update(_pParticleComponent.get(), dt);
	_pBuoyancyForce->Update(_pParticleComponent.get(), dt);
	_timer += dt;

	if (_timer > _timeBetweenHeightChanges) {
		_timer = 0.0f;

		int r = rand() % 100 + 1;

		if (r <= 50) {
			_currentWaveHeight -= _variance;
		}
		else {
			_currentWaveHeight += _variance;
		}

		if (_currentWaveHeight > _maxHeight) {
			_currentWaveHeight = _maxHeight;
		}
		else if (_currentWaveHeight < _minHeight) {
			_currentWaveHeight = _minHeight;
		}

		_pBuoyancyForce->SetWaterHeight(_currentWaveHeight);
		_pGameObjects[1]->GetTransform().SetPosition(0.0f, _currentWaveHeight, 0.0f);
	}
}

void Level7::DrawUI()
{
	ImGui::Begin("Test 7");

	OutputVector3("Object Position: ", _pGameObjects[0]->GetTransform().GetPosition());
	OutputVector3("Wave Position: ", _pGameObjects[1]->GetTransform().GetPosition());

	ResetButton();

	ImGui::End();
}
