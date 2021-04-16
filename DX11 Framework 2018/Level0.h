#pragma once
#include "Scene.h"
class Level0 : public Scene
{
public:
	Level0(std::shared_ptr<Graphics> gfx, const char* levelName) : Scene(gfx, levelName) {}

	void DrawUI() override;


	void Update(float dt) override;
};

