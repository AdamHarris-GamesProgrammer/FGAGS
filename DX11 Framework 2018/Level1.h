#pragma once
#include "Level.h"
#include "Particle.h"

class Level1 : public Level
{
public:
	Level1(Graphics* gfx, const char* levelName) : Level(gfx, levelName)
	{
		LoadLevel();
	}

	void LoadLevel() override;


	void Update(float dt) override;

private:
	ParticleComponent* _pParticleComponent;

	void DrawUI() override;

};

