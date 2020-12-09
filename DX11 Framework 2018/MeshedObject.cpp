#include "MeshedObject.h"

MeshedObject::MeshedObject(Graphics* gfx, const char* filepath)
	: GameObject(gfx)
{
	Load(filepath);
}

MeshedObject::MeshedObject(Graphics* gfx) : GameObject(gfx) {}

bool MeshedObject::TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection)
{
	float distance = 0.0f;

	if (mBoundingSphere.Radius == 0.0f) return false;

	if (mBoundingSphere.Intersects(XMLoadFloat4(&rayOrigin), XMLoadFloat4(&rayDirection), distance)) {
		return true;
	}

	return false;
}

void MeshedObject::Load(const char* filepath)
{
	mMesh = OBJLoader::Load(filepath, mGfx->GetDevice(),mGfx->GetDeviceContext(),false);

	mVertexBuffer = new VertexBuffer(mGfx->GetDevice(), mGfx->GetDeviceContext(), mMesh.Vertices);
	mIndexBuffer = new IndexBuffer(mGfx->GetDevice(), mGfx->GetDeviceContext(), mMesh.Indices);

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
