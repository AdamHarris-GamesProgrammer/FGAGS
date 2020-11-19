#pragma once
#include "GameObject.h"
#include "VertexBuffer.h"

class Plane : public GameObject {
public:
	Plane(Graphics* gfx) : GameObject(gfx) {
		//vBuffer = new VertexBuffer(gfx);
	}

	void Make(float width, float depth, UINT m, UINT n);

	void Draw() override;

private:
	VertexBuffer* vBuffer;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	UINT indexCount = 0;
};
