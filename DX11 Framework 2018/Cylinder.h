#pragma once
#include "GameObject.h"

class Cylinder : public GameObject
{
public:
	Cylinder::Cylinder() : GameObject() {}

	Cylinder::Cylinder(Graphics* gfx) : GameObject(gfx, "Assets/Models/cylinder.obj") {}
};