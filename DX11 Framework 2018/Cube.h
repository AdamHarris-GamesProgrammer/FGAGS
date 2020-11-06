#pragma once
#include "GameObject.h"


class Cube : public GameObject
{
public:
	Cube::Cube() : GameObject() {}

	Cube::Cube(Graphics* gfx) : GameObject(gfx, "Assets/Models/cube.obj") {}
};

