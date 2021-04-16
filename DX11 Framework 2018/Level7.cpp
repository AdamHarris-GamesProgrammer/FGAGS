#include "Level7.h"

void Level7::LoadLevel()
{
	Scene::LoadLevel();

	if (_pParticleComponent == nullptr) 
		_pParticleComponent = std::make_unique<ParticleComponent>(_pGameObjects[0]);
	else
		_pParticleComponent->SetOwner(_pGameObjects[0]);
	

	if (_pGravityForce == nullptr) 
		_pGravityForce = std::make_unique<GravityForceGenerator>(Vector3(0.0f, -9.81f, 0.0f));

	if (_pBuoyancyForce == nullptr) 
		_pBuoyancyForce = std::make_unique<BuoyancyForceGenerator>(7.0f, 5.0f, 7.0f, 1000.0f);

	//Stores the height of the wave into the wave height variable
	_currentWaveHeight = _pGameObjects[1]->GetTransform().GetPosition().y;

	//Resets the timer
	_timer = 0.0f;
}

void Level7::Update(float dt)
{
	//updates the gravity and buoyancy force generators
	_pGravityForce->Update(_pParticleComponent.get(), dt);
	_pBuoyancyForce->Update(_pParticleComponent.get(), dt);

	//Adds the delta time to the timer
	_timer += dt;

	//if the timer is up
	if (_timer > _timeBetweenHeightChanges) {
		_timer = 0.0f;

		//Generate a number between 1 and 100
		int r = rand() % 100 + 1;

		//if the number is less than 50 the wave is lowered, if it is greater than 50 the wave is lifted up
		if (r <= 50) _currentWaveHeight -= _variance;
		else _currentWaveHeight += _variance;
		
		//Clamp to the max value
		if (_currentWaveHeight > _maxHeight)  _currentWaveHeight = _maxHeight;
		//Clamp to the min value
		else if (_currentWaveHeight < _minHeight) _currentWaveHeight = _minHeight;
	
		//Set the water height for the buoyancy generator
		_pBuoyancyForce->SetWaterHeight(_currentWaveHeight);

		//Sets the position of our object
		_pGameObjects[1]->GetTransform().SetPosition(0.0f, _currentWaveHeight, 0.0f);
	}
}

void Level7::DrawUI()
{
	ImGui::Begin("Test 7");

	ImGui::Text("This demo uses a basic buoyancy force generator to float a cube in a wave that changes height.");
	ImGui::Text("Uses the particle model ");
	
	ResetButton();

	ImGui::End();
}
