#pragma once
#include "GameObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

/// <summary>
/// Meshed Object class is used to represent any objects that are loaded from a obj file
/// </summary>
class MeshedObject : public GameObject
{
public:
	MeshedObject(Graphics* gfx, const char* filepath);
	MeshedObject(Graphics* gfx);
	MeshedObject() {}

	//Overrides the TestCollision method 
	bool TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection) override;

	//Loads the model based on the type of parameter passed in
	void Load(const char* filepath);
	void Load(std::string& filepath);

	//Initializes the bounding sphere with the radius of the model
	void InitializeBoundingSphere();

private:
	MeshData mMesh;
};

