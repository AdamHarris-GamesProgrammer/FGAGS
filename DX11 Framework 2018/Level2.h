#pragma once
#include "Scene.h"
#include "GravityForceGenerator.h"
#include "RestingForceGenerator.h"
#include <memory>

class Level2 : public Scene
{
public:
	Level2(Graphics* gfx, const char* filename) : Scene(gfx, filename) {}

	void LoadLevel() override;


	void ExitLevel() override;

	void Update(float dt) override;

private:
	std::unique_ptr<GravityForceGenerator> _pGravityGenerator = nullptr;
	std::unique_ptr<RestingForceGenerator> _pRestingGenerator = nullptr;

	void DrawUI() override;


	RigidbodyComponent* _pRb;
};

