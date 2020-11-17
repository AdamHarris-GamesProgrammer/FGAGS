#pragma once
#include "MeshedObject.h"


class Cube : public MeshedObject
{
public:
	Cube::Cube() : MeshedObject() {}

	Cube::Cube(Graphics* gfx) : MeshedObject(gfx, "Assets/Models/cube.obj") {}
};

