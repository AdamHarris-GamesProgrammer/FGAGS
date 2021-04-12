#pragma once
#include "Scene.h"
#include "Particle.h"
#include "DragForceGenerator.h"

class Level1 : public Scene
{
public:
	Level1(Graphics* gfx, const char* levelName) : Scene(gfx, levelName) {}

	void LoadLevel() override;

	void PollInput(float dt) override;

	void Update(float dt) override;

private:
	bool _shouldMove = false;

	std::unique_ptr<ParticleComponent> _pParticleComponent;
	std::unique_ptr<DragForceGenerator> _pParticalDrag;

	void DrawUI() override;

};

