#pragma once
#include "GameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class MeshedObject : public GameObject
{
public:
	MeshedObject(Graphics* gfx, const char* filepath);
	MeshedObject(Graphics* gfx);
	MeshedObject() {}

	bool TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection) override;

	void Load(const char* filepath);
	void Load(std::string& filepath);

	void InitializeBoundingSphere();

private:
	//Mesh
	MeshData mMesh;
};

