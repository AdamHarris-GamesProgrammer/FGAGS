#include "GameObject.h"

GameObject::GameObject()
{
	Initialize();
}

GameObject::GameObject(Graphics* gfx, char* filepath) : gfx(gfx)
{
	mMesh = OBJLoader::Load(filepath, this->gfx->_pd3dDevice, false);
	
	Initialize();
}

void GameObject::Update(float dt)
{
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);

	transformMatrix = XMMatrixMultiply(transformMatrix,
		XMMatrixScaling(mScale.x, mScale.y, mScale.z) *
		XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z) *
		XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z)
	);

	XMStoreFloat4x4(&mTransform, transformMatrix);
}

void GameObject::Draw()
{

	gfx->SwitchVertexBuffer(mMesh.VertexBuffer);
	gfx->SwitchIndexBuffer(mMesh.IndexBuffer);
	gfx->UpdateBuffers(mTransform, 0.0f);
	gfx->Draw(mMesh.IndexCount);
}

DirectX::XMFLOAT4X4 GameObject::GetTransform()
{
	return mTransform;
}

void GameObject::SetTransform(XMMATRIX transform)
{
	XMStoreFloat4x4(&mTransform, transform);
}

void GameObject::SetTransform(XMFLOAT4X4 transform)
{
	mTransform = transform;
}

void GameObject::SetPosition(XMFLOAT4 position)
{
	mPosition = position;
}

void GameObject::SetRotation(XMFLOAT4 rotation)
{
	mRotation = rotation;
}

void GameObject::SetScale(XMFLOAT4 scale)
{
	mScale = scale;
}

void GameObject::Initialize()
{
	XMStoreFloat4x4(&mTransform, XMMatrixIdentity());
	mPosition = XMFLOAT4(0.0f,0.0f,0.0f,0.0f);
	mRotation = XMFLOAT4(0.0f,0.0f,0.0f,0.0f);
	mScale = XMFLOAT4(1.0f,1.0f,1.0f,0.0f);

}
