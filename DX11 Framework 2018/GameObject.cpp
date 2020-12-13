#include "GameObject.h"

GameObject::GameObject()
{
	Initialize();
}


GameObject::GameObject(Graphics* gfx)
{
	this->pGfx = gfx;
	Initialize();
}

GameObject::~GameObject()
{
	delete pVertexShader;
	delete pPixelShader;
	delete pVertexShader;
	delete pIndexBuffer;

	pVertexBuffer = nullptr;
	pVertexShader = nullptr;
	pPixelShader = nullptr;
	pIndexBuffer = nullptr;
}

void GameObject::Update(float dt)
{
	//Stores the objects new world space location
	//Updates the bounding spheres position
	CalculateTransform();
	mBoundingSphere.Center = mPosition;
}

//Collision method may be different each type of object
bool GameObject::TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection)
{
	return false;
}


void GameObject::CalculateTransform()
{
	//Loads the transform matrix
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);

	//Sets the scale, position and rotation matrices
	XMMATRIX objectScale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	XMMATRIX objectPosition = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	XMMATRIX objectRotation = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);

	//Calculates the transform
	XMMATRIX calculatedTransform = XMMatrixMultiply(objectScale, objectPosition) * objectRotation;

	//Stores the transform
	XMStoreFloat4x4(&mTransform, calculatedTransform);
}

void GameObject::Draw()
{
	//Binds the shaders and buffers
	pVertexShader->Bind();
	pPixelShader->Bind();
	pVertexBuffer->Bind();
	pIndexBuffer->Bind();

	//Sets the textures for the shaders
	if (mHasTextures) {
		pGfx->GetDeviceContext()->PSSetShaderResources(0, pTextures.size(), &pTextures[0]);
	}

	//Sets the constant buffer
	pGfx->SetConstantBuffer();

	//Sends the objects material and transform to the graphics class for the updated constant buffer
	pGfx->SetObjectBuffers(mMaterial, mTransform);

	//Draws the object
	pGfx->Draw(pIndexBuffer->GetIndexCount());
}


void GameObject::CreateTexture(const wchar_t* path)
{
	ID3D11ShaderResourceView* texture;

	mHasTextures = true;

	//Creates and adds the texture to the texture vector
	CreateDDSTextureFromFile(pGfx->GetDevice(), path, nullptr, &texture);
	pTextures.push_back(texture);
}


void GameObject::Initialize()
{
	XMStoreFloat4x4(&mTransform, XMMatrixIdentity());
	//Initializes the position and rotation to world origin
	mPosition = XMFLOAT3(0.0f,0.0f,0.0f);
	mRotation = XMFLOAT3(0.0f,0.0f,0.0f);
	//Sets the default scale
	mScale = XMFLOAT3(1.0f,1.0f,1.0f);
	
	//Sets sensible default values for the material
	mMaterial.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 10.0f);

	//Sets the radius and position of the bounding sphere
	mBoundingSphere.Radius = 0.0f;
	mBoundingSphere.Center = GetPosition();

	//Creates a temporary layout array for the vertex shader
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Sets the vertex shader
	pVertexShader = new VertexShader(pGfx->GetDevice(), pGfx->GetDeviceContext(), L"VertexShaderUtilities.fx", layout, 3);
}

#pragma region Getters
std::string GameObject::GetName() const
{
	return mName;
}

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
void GameObject::SetShader(WCHAR* path)
{
	pPixelShader = new PixelShader(pGfx->GetDevice(), pGfx->GetDeviceContext(), path);
}
void GameObject::SetName(std::string& name)
{
	mName = name;
}

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
