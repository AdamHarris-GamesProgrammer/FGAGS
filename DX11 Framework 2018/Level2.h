#pragma once
#include "Level.h"
#include "Plane.h"

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
	Plane* pGroundPlane = nullptr;

	void DrawUI() override;

};

