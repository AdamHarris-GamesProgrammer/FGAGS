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
	XMStoreFloat4x4(&mTransform, CalculateTransform());
}

DirectX::XMMATRIX GameObject::CalculateTransform()
{
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);

	

	transformMatrix = XMMatrixMultiply(transformMatrix,
		XMMatrixScaling(mScale.x, mScale.y, mScale.z) *
		XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z) *
		XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z)
	);

	return transformMatrix;
}

void GameObject::Draw()
{

	gfx->SwitchVertexBuffer(mMesh.VertexBuffer);
	gfx->SwitchIndexBuffer(mMesh.IndexBuffer);
	gfx->UpdateBuffers(mTransform);
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

void GameObject::SetPosition(XMFLOAT3 position)
{
	mPosition = position;
}

void GameObject::SetRotation(XMFLOAT3 rotation)
{
	mRotation = rotation;
}

void GameObject::SetScale(XMFLOAT3 scale)
{
	mScale = scale;
}

void GameObject::Initialize()
{
	XMStoreFloat4x4(&mTransform, XMMatrixIdentity());
	mPosition = XMFLOAT3(0.0f,0.0f,0.0f);
	mRotation = XMFLOAT3(0.0f,0.0f,0.0f);
	mScale = XMFLOAT3(1.0f,1.0f,1.0f);
	

}
