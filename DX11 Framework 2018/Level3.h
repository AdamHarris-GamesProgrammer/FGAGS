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
	//Rotational power modifier
	float _rotationPower = 10.0f;
	//Holds the normalized device coordinates for where we click on the screen
	float _normalizedCoords[2] = { 0.0f, 0.0f };

	//Holds a rigidbody component
	std::unique_ptr<RigidbodyComponent> _pRb;

	//Checks if we are in menus, we dont want to spin the cube if we are tweaking the rotation power value
	bool _inMenus = false;

};

