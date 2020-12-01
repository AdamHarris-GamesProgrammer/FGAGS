#pragma once
#include "GameObject.h"

class MeshedObject : public GameObject
{
public:
	MeshedObject(Graphics* gfx, const char* filepath);
	MeshedObject(Graphics* gfx);
	MeshedObject();

	void Draw() override;
	void Update(float dt) override;
	bool TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection) override;

	void Load(const char* filepath);
	void Load(std::string& filepath);

private:
	//Mesh
	MeshData mMesh;
};

