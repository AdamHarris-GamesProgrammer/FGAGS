#include "GameObject.h"

GameObject::GameObject() 
{
	Initialize();
}



GameObject::GameObject(Graphics* gfx)
{
	_pGfx = gfx;

	_name = "GameObject";

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

	pTextures.clear();
}

void GameObject::Update(float dt)
{
	RigidbodyComponent* rb = dynamic_cast<RigidbodyComponent*>(GetComponent(Rigidbody));

	if (rb) {
		rb->SetPosition(_transform.GetPosition());
	}
	else
	{
		Debug::Print("Rigidbody is null\n");
	}

	Object::Update(dt);
}

void GameObject::UpdateTransforms()
{
	RigidbodyComponent* rb = dynamic_cast<RigidbodyComponent*>(GetComponent(Rigidbody));

	if (rb) {
		_transform.SetPosition(rb->GetPosition());
		_transform.SetRotation(rb->GetOrientation().Identity());



		float transform[16];
		rb->GetTransform().DirectXArray(transform);

		_transform.SetTransform(XMFLOAT4X4(transform));
	}


	//Updates transform
	_transform.Update();
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
		_pGfx->GetDeviceContext()->PSSetShaderResources(0, pTextures.size(), &pTextures[0]);
	}

	//Sets the constant buffer
	_pGfx->SetConstantBuffer();

	//Sends the objects material and transform to the graphics class for the updated constant buffer
	_pGfx->SetObjectBuffers(mMaterial, _transform.GetTransform());

	//Draws the object
	_pGfx->Draw(pIndexBuffer->GetIndexCount());
}


void GameObject::CreateTexture(const wchar_t* path)
{
	ID3D11ShaderResourceView* texture;

	mHasTextures = true;

	//Creates and adds the texture to the texture vector
	CreateDDSTextureFromFile(_pGfx->GetDevice(), path, nullptr, &texture);
	pTextures.push_back(texture);
}


void GameObject::Initialize()
{
	//Sets the vertex shader
	pVertexShader = new VertexShader(_pGfx->GetDevice(), _pGfx->GetDeviceContext());
}

#pragma region Getters
Material& GameObject::GetMaterial() 
{
	return mMaterial;
}

#pragma endregion

#pragma region Setters
void GameObject::SetShader(WCHAR* path)
{
	pPixelShader = new PixelShader(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), path);
}

#pragma endregion
