#pragma once
#include "Level.h"
#include "Particle.h"
#include "DragForceGenerator.h"

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
	std::unique_ptr<ParticleComponent> _pParticleComponent;
	std::unique_ptr<DragForceGenerator> _pParticalDrag;

	void DrawUI() override;

};

