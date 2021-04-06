#pragma once
#include "Level.h"
#include "Particle.h"
#include "BuoyancyForceGenerator.h"
#include "GravityForceGenerator.h"
#include <memory>

class Level7 : public Level
{
public:
	Level7(Graphics* gfx, const char* levelName) : Level(gfx, levelName) {}

	void LoadLevel() override;

	void Update(float dt) override;

private:
	std::unique_ptr<ParticleComponent> _pParticleComponent = nullptr;
	std::unique_ptr<BuoyancyForceGenerator> _pBuoyancyForce = nullptr;
	std::unique_ptr<GravityForceGenerator> _pGravityForce = nullptr;

	float _variance = 0.05f;
	float _currentWaveHeight;
	float _maxHeight = 4.0f;
	float _minHeight = -4.0f;

	float _timeBetweenHeightChanges = 0.2f;
	float _timer;

	void DrawUI() override;

};

