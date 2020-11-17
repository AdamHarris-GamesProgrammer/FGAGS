#pragma once
#include "MeshedObject.h"

class Sphere : public MeshedObject {
public:
	Sphere() : MeshedObject() {}

	Sphere(Graphics* gfx) : MeshedObject(gfx, "Assets/Models/sphere.obj") {}
};
