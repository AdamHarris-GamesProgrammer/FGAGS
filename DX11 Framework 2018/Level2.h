#pragma once
#include "Level.h"
#include "GravityForceGenerator.h"
#include "RestingForceGenerator.h"

class Level2 : public Level
{
public:
	Level2(Graphics* gfx, const char* filename) : Level(gfx, filename)
	{
		LoadLevel();
	}

	void LoadLevel() override;


	void ExitLevel() override;


	void PollInput(float dt) override;


	void Update(float dt) override;


	void Reset() override;

private:
	GravityForceGenerator* _pGravityGenerator = nullptr;
	RestingForceGenerator* _pRestingGenerator = nullptr;

	void DrawUI() override;

};

