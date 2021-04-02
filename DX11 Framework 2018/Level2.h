#pragma once
#include "Level.h"
#include "GravityForceGenerator.h"
#include "RestingForceGenerator.h"
#include <memory>

class Level2 : public Level
{
public:
	Level2(Graphics* gfx, const char* filename) : Level(gfx, filename)
	{
		LoadLevel();
	}

	void LoadLevel() override;


	void ExitLevel() override;

	void Update(float dt) override;

private:
	std::unique_ptr<GravityForceGenerator> _pGravityGenerator = nullptr;
	std::unique_ptr<RestingForceGenerator> _pRestingGenerator = nullptr;

	void DrawUI() override;


	RigidbodyComponent* _pRb;
};

