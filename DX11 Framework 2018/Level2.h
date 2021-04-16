#pragma once
#include "Scene.h"
#include "GravityForceGenerator.h"
#include "RestingForceGenerator.h"
#include <memory>

class Level2 : public Scene
{
public:
	Level2(std::shared_ptr<Graphics> gfx, const char* filename) : Scene(gfx, filename) {}

	void LoadLevel() override;


	void Update(float dt) override;

private:
	//This level needs to generate gravity and a resting force to an object
	std::unique_ptr<GravityForceGenerator> _pGravityGenerator = nullptr;
	std::unique_ptr<RestingForceGenerator> _pRestingGenerator = nullptr;

	//Stores the rigidbody component for this level
	std::unique_ptr<RigidbodyComponent> _pRb = nullptr;

	void DrawUI() override;

};

