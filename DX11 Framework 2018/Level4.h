#pragma once
#include "Level.h"
class Level4 : public Level
{
public:
	Level4(Graphics* gfx, const char* filename) : Level(gfx, filename) {
		LoadLevel();
	}

	void LoadLevel() override;


	void ExitLevel() override;


	void PollInput(float dt) override;


	void Update(float dt) override;


	void Reset() override;

private:
	void DrawUI() override;

};

