#include "MeshedObject.h"

//Loads the object immediately if a filepath is given
MeshedObject::MeshedObject(Graphics* gfx, const char* filepath)
	: GameObject(gfx)
{
	Load(filepath);
	_name = "MeshedObject";
}

MeshedObject::MeshedObject(Graphics* gfx) : GameObject(gfx) {}

bool MeshedObject::TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection)
{
	float distance = 0.0f;

	//If the bounding sphere has a radius of 0 then return false as the object does not detect collisions
	if (mBoundingSphere.Radius == 0.0f) return false;

	//Checks for a intersect
	if (mBoundingSphere.Intersects(XMLoadFloat4(&rayOrigin), XMLoadFloat4(&rayDirection), distance)) {
		return true;
	}

	return false;
}

void MeshedObject::Load(const char* filepath)
{
	//Loads the mesh data
	mMesh = OBJLoader::Load(filepath, pGfx->GetDevice(),pGfx->GetDeviceContext(),false);

	//Sets the vertex and index buffers
	pVertexBuffer = new VertexBuffer(pGfx->GetDevice(), pGfx->GetDeviceContext(), mMesh.Vertices);
	pIndexBuffer = new IndexBuffer(pGfx->GetDevice(), pGfx->GetDeviceContext(), mMesh.Indices);

	//Clears the vertices and indices array in order to reduce memory overhead
	mMesh.Vertices.clear();
	mMesh.Indices.clear();

}

void MeshedObject::Load(std::string& filepath)
{
	Load(filepath.c_str());
}

void MeshedObject::InitializeBoundingSphere()
{
	mBoundingSphere.Radius = mMesh.Radius;
}
