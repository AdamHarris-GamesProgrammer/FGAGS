#pragma once
#include "GameObject.h"

class Donut : public GameObject
{
public:
	Donut::Donut() : GameObject() {}

	Donut::Donut(Graphics* gfx) : GameObject(gfx, "Assets/Models/donut.obj") {}
};