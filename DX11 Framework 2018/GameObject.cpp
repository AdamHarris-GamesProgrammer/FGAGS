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
	delete pVertexBuffer;
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
	transform.Update();
	mBoundingSphere.Center = transform.GetPosition();
}

//Collision method may be different each type of object
bool GameObject::TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection)
{
	return false;
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
	pGfx->SetObjectBuffers(mMaterial, transform.GetTransform());

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
	//Initializes the position and rotation to world origin
	transform = Transform();
	
	//Sets sensible default values for the material
	mMaterial.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mMaterial.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMaterial.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 10.0f);

	//Sets the radius and position of the bounding sphere
	mBoundingSphere.Radius = 0.0f;
	mBoundingSphere.Center = transform.GetPosition();

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


Material GameObject::GetMaterial() const
{
	return mMaterial;
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

#pragma endregion
