#pragma once
#include "Scene.h"
#include "Rigidbody.h"
#include <memory>

class Level3 : public Scene
{
public:
	Level3(Graphics* gfx, const char* filename) : Scene(gfx, filename) {}

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

