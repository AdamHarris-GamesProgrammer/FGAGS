#pragma once
#include "Level.h"
class Level1 : public Level
{
public:
	Level1(Graphics* gfx, const char* levelName) : Level(gfx, levelName)
	{
		LoadLevel();
	}

	void LoadLevel() override;


	void PollInput(float dt) override;


	void Update(float dt) override;


	void Reset() override;

private:
	void DrawUI() override;

};

