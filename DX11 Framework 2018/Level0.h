#pragma once
#include "Level.h"
class Level0 : public Level
{
public:
	Level0(Graphics* gfx, const char* levelName) : Level(gfx, levelName) {}

	void DrawUI() override;


	void Update(float dt) override;
};

