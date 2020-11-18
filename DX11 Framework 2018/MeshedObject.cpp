#include "MeshedObject.h"

MeshedObject::MeshedObject(Graphics* gfx, char* filepath)
	: GameObject(gfx)
{
	mMesh = OBJLoader::Load(filepath, mGfx->GetDevice(), false);
}

MeshedObject::MeshedObject()
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
