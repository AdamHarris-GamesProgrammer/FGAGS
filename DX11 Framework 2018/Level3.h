#pragma once
#include "Level.h"
#include <memory>

class Level3 : public Level
{
public:
	Level3(Graphics* gfx, const char* filename) : Level(gfx, filename) {}

	void LoadLevel() override;


	void ExitLevel() override;

	void Update(float dt) override;

private:
	void DrawUI() override;

private:
	float _rotationPower = 10.0f;
	float _normalizedCoords[2];

	std::unique_ptr<RigidbodyComponent> _pRb;

};

