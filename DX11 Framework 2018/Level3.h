#pragma once
#include "Level.h"
class Level3 : public Level
{
public:
	Level3(Graphics* gfx, const char* filename) : Level(gfx, filename) {
		LoadLevel();
	}

	void LoadLevel() override;


	void ExitLevel() override;


	void PollInput(float dt) override;


	void Update(float dt) override;


	void Reset() override;

private:
	void DrawUI() override;

private:
	float _rotationPower = 10.0f;
	float _normalizedCoords[2];

};

