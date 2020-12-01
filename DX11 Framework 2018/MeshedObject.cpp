#include "MeshedObject.h"

MeshedObject::MeshedObject(Graphics* gfx, const char* filepath)
	: GameObject(gfx)
{
	Load(filepath);


}

MeshedObject::MeshedObject()
{

}

MeshedObject::MeshedObject(Graphics* gfx)
	: GameObject(gfx)
{

}

void MeshedObject::Draw()
{
	mShader->BindShaders();

	vb->Bind();
	ib->Bind();

	if (hasTextures)
	{
		mGfx->BindTextures(0, mTextures.size(), mTextures);
	}

	mGfx->UpdateBuffers(mMaterial, mTransform);
	mGfx->Draw(ib->GetIndexCount());
}

void MeshedObject::Update(float dt)
{
	GameObject::Update(dt);
	mBoundingSphere.Center = GetPosition();

}

bool MeshedObject::TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection)
{
	float distance = 0.0f;
	if (mBoundingSphere.Intersects(XMLoadFloat4(&rayOrigin), XMLoadFloat4(&rayDirection), distance)) {
		return true;
	}

	return false;
}

void MeshedObject::Load(const char* filepath)
{
	mMesh = OBJLoader::Load(filepath, mGfx->GetDevice(),mGfx->GetDeviceContext(),true);

	vb = new VertexBuffer(mGfx->GetDevice(), mGfx->GetDeviceContext(), mMesh.Vertices);
	ib = new IndexBuffer(mGfx->GetDevice(), mGfx->GetDeviceContext(), mMesh.Indices);

	mBoundingSphere.Radius = mMesh.Radius;
}

void MeshedObject::Load(std::string& filepath)
{
	Load(filepath.c_str());
}
