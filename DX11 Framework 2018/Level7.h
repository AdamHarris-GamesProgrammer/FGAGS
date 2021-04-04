#pragma once
#include "Level.h"
#include "Particle.h"
#include "ParticleBungee.h"
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
	void DrawUI() override;

	std::unique_ptr<ParticleComponent> _pParticleAnchor = nullptr;
	std::unique_ptr<ParticleComponent> _pParticleA = nullptr;
	std::unique_ptr<ParticleBungee> _pParticleBungee = nullptr;


};

