#pragma once
#include "GameObject.h"

class Sphere : public GameObject {
public:
	Sphere() : GameObject() {}

	Sphere(Graphics* gfx) : GameObject(gfx, "Assets/Models/sphere.obj") {}
};
