#pragma once
#include "GameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RendererComponent.h"

class Plane {
public:
	//B3
	//Calculates the vertices and indices needed for the model
	void Make(float width, float depth, UINT m, UINT n, RendererComponent* renderer);
};
