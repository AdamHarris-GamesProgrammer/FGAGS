#pragma once
#include "GameObject.h"
class MeshedObject : public GameObject
{
public:
	MeshedObject(Graphics* gfx, char* filepath);
	MeshedObject();

	void Draw() override;

private:
	//Mesh
	MeshData mMesh;
};

