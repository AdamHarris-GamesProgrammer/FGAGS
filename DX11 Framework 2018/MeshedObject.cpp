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
	mGfx->SwitchVertexBuffer(mMesh.VertexBuffer);
	mGfx->SwitchIndexBuffer(mMesh.IndexBuffer);

	if (hasTextures)
	{
		mGfx->BindTextures(0, mTextures.size(), mTextures);
	}

	mGfx->UpdateBuffers(mTransform);
	mGfx->Draw(mMesh.IndexCount);
}

void MeshedObject::Load(const char* filepath)
{
	mMesh = OBJLoader::Load(filepath, mGfx->GetDevice(), true);
}

void MeshedObject::Load(std::string& filepath)
{
	Load(filepath.c_str());
}
