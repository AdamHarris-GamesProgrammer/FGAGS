#pragma once
#include "GameObject.h"

class Plane : public GameObject {
public:
	Plane(Graphics* gfx) : GameObject(gfx) {
	}

	void Make(float width, float depth, UINT m, UINT n);

	void Draw() override;

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	UINT indexCount = 0;
};
