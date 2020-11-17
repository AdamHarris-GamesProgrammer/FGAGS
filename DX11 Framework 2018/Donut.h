#pragma once
#include "MeshedObject.h"

class Donut : public MeshedObject
{
public:
	Donut::Donut() : MeshedObject() {}

	Donut::Donut(Graphics* gfx) : MeshedObject(gfx, "Assets/Models/donut.obj") {}
};