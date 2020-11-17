#pragma once
#include "MeshedObject.h"

class Cylinder : public MeshedObject
{
public:
	Cylinder::Cylinder() : MeshedObject() {}

	Cylinder::Cylinder(Graphics* gfx) : MeshedObject(gfx, "Assets/Models/cylinder.obj") {}
};