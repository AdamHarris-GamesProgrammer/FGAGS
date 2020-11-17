#include "GameObject.h"

GameObject::GameObject()
{
	Initialize();
}


GameObject::GameObject(Graphics* gfx)
{
	this->mGfx = gfx;
	Initialize();
}

void GameObject::Update(float dt)
{
	XMStoreFloat4x4(&mTransform, CalculateTransform());
}

DirectX::XMMATRIX GameObject::CalculateTransform()
{
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);

	XMMATRIX objectScale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	XMMATRIX objectPosition = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	XMMATRIX objectRotation = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);

	XMMATRIX calculatedTransform = XMMatrixMultiply(objectScale, objectPosition) * objectRotation;

	XMStoreFloat4x4(&mTransform, calculatedTransform);

	return calculatedTransform;
}

void GameObject::Draw()
{

}


void GameObject::CreateTexture(wchar_t* path)
{
	ID3D11ShaderResourceView* texture;

	hasTextures = true;

	mGfx->CreateTexture(path, &texture);
	mTextures.push_back(texture);
}

void GameObject::Initialize()
{
	XMStoreFloat4x4(&mTransform, XMMatrixIdentity());
	mPosition = XMFLOAT3(0.0f,0.0f,0.0f);
	mRotation = XMFLOAT3(0.0f,0.0f,0.0f);
	mScale = XMFLOAT3(1.0f,1.0f,1.0f);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mShader = new Shaders(mGfx, L"DX11 Framework.fx", layout, 3);
	mShader->InitializeShaders();
}

#pragma region Getters
DirectX::XMFLOAT3 GameObject::GetPosition()
{
	return XMFLOAT3(mTransform._41, mTransform._42, mTransform._43);
}

DirectX::XMFLOAT3 GameObject::GetRotation()
{
	return mRotation;
}

DirectX::XMFLOAT3 GameObject::GetScale()
{
	return mScale;
}
DirectX::XMFLOAT4X4 GameObject::GetTransform()
{
	return mTransform;
}
#pragma endregion

#pragma region Setters
void GameObject::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void GameObject::SetRotation(XMFLOAT3 rotation)
{
	mRotation = rotation;
}

void GameObject::SetRotation(float x, float y, float z)
{
	mRotation = XMFLOAT3(x, y, z);
}

void GameObject::SetScale(XMFLOAT3 scale)
{
	mScale = scale;
}

void GameObject::SetScale(float x, float y, float z)
{
	mScale = XMFLOAT3(x, y, z);
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

#pragma endregion
