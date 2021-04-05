#pragma once
#include "Level.h"
#include "Particle.h"
#include "BuoyancyForceGenerator.h"
#include "GravityForceGenerator.h"
#include "RestingForceGenerator.h"
#include <memory>

class Level7 : public Level
{
public:
	Level7(Graphics* gfx, const char* levelName) : Level(gfx, levelName) {
		LoadLevel();
	}

	void LoadLevel() override;

	void Update(float dt) override;

private:
	std::unique_ptr<ParticleComponent> _pParticleComponent = nullptr;
	std::unique_ptr<BuoyancyForceGenerator> _pBuoyancyForce = nullptr;
	std::unique_ptr<GravityForceGenerator> _pGravityForce = nullptr;
	std::unique_ptr<RestingForceGenerator> _pRestingForce = nullptr;

	void DrawUI() override;

};

