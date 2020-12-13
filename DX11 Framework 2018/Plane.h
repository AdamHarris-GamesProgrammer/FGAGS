#pragma once
#include "GameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Plane : public GameObject {
public:
	Plane(Graphics* gfx) : GameObject(gfx) {}

	//Calculates the vertices and indices needed for the model
	void Make(float width, float depth, UINT m, UINT n);
};
