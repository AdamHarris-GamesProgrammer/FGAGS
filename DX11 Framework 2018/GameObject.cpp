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
	mBoundingSphere.Center = mPosition;
}

bool GameObject::TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection)
{
	return false;
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
	mVertexShader->Bind();
	mPixelShader->Bind();
	mVertexBuffer->Bind();
	mIndexBuffer->Bind();

	if (hasTextures) {
		mGfx->GetDeviceContext()->PSSetShaderResources(0, mTextures.size(), &mTextures[0]);
	}

	mGfx->SetConstantBuffer();

	mGfx->UpdateBuffers(mMaterial, mTransform);
	mGfx->Draw(mIndexBuffer->GetIndexCount());
}


void GameObject::CreateTexture(const wchar_t* path)
{
	ID3D11ShaderResourceView* texture;

	hasTextures = true;

	CreateDDSTextureFromFile(mGfx->GetDevice(), path, nullptr, &texture);
	mTextures.push_back(texture);
}

void GameObject::SetName(std::string& name)
{
	mName = name;
}

std::string GameObject::GetName()
{
	return mName;
}

void GameObject::SetShader(WCHAR* path)
{
	mPixelShader = new PixelShader(mGfx->GetDevice(), mGfx->GetDeviceContext(), path);
}

void GameObject::Initialize()
{
	XMStoreFloat4x4(&mTransform, XMMatrixIdentity());
	mPosition = XMFLOAT3(0.0f,0.0f,0.0f);
	mRotation = XMFLOAT3(0.0f,0.0f,0.0f);
	mScale = XMFLOAT3(1.0f,1.0f,1.0f);
	
	mMaterial.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 10.0f);

	mBoundingSphere.Radius = 0.0f;
	mBoundingSphere.Center = GetPosition();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	mVertexShader = new VertexShader(mGfx->GetDevice(), mGfx->GetDeviceContext(), L"VertexShaderUtilities.fx", layout, 3);
}

#pragma region Getters
DirectX::XMFLOAT3 GameObject::GetPosition() const
{
	return XMFLOAT3(mTransform._41, mTransform._42, mTransform._43);
}

DirectX::XMFLOAT3 GameObject::GetRotation() const
{
	return mRotation;
}

DirectX::XMFLOAT3 GameObject::GetScale() const
{
	return mScale;
}

Material GameObject::GetMaterial() const
{
	return mMaterial;
}

DirectX::XMFLOAT4X4 GameObject::GetTransform() const
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

void GameObject::SetMaterialDiffuse(XMFLOAT4 color)
{
	mMaterial.Diffuse = color;
}

void GameObject::SetMaterialAmbient(XMFLOAT4 color)
{
	mMaterial.Ambient = color;
}

void GameObject::SetMaterialSpecular(XMFLOAT4 color)
{
	mMaterial.Specular = color;
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
