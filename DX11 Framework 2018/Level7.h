#pragma once
#include "Scene.h"
#include "Particle.h"
#include "BuoyancyForceGenerator.h"
#include "GravityForceGenerator.h"
#include <memory>

class Level7 : public Scene
{
public:
	Level7(Graphics* gfx, const char* levelName) : Scene(gfx, levelName) {}

	void LoadLevel() override;

	void Update(float dt) override;

private:
	//Using unique pointers to handle deallocation of memory
	//Holds the particle we will be manipulating
	std::unique_ptr<ParticleComponent> _pParticleComponent = nullptr;

	//Holds the force generators we are using to manipulate the particle
	std::unique_ptr<BuoyancyForceGenerator> _pBuoyancyForce = nullptr;
	std::unique_ptr<GravityForceGenerator> _pGravityForce = nullptr;

	//These variables control the current wave height which changes every 0.2 seconds
	float _variance = 0.05f;
	float _currentWaveHeight = 0.0f;
	float _maxHeight = 4.0f;
	float _minHeight = -4.0f;

	//Holds the timer variables for changing wave height
	float _timeBetweenHeightChanges = 0.2f;
	float _timer = 0.0f;

	
	void DrawUI() override;

};

