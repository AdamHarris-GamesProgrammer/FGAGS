#include "GameObject.h"

GameObject::GameObject() 
{
}



GameObject::GameObject(Graphics* gfx)
{
	_pGfx = gfx;

	_name = "GameObject";

	pVertexShader = new VertexShader(_pGfx->GetDevice(), _pGfx->GetDeviceContext());
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
	_pGfx->SetObjectBuffers(mMaterial, _pTransform->GetDirectXTransform());

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


#pragma region Getters
Material& GameObject::GetMaterial() 
{
	return mMaterial;
}
#pragma endregion

#pragma region Setters
void GameObject::SetShader(WCHAR* path)
{
	if (pVertexShader == nullptr) {
		pVertexShader = new VertexShader(_pGfx->GetDevice(), _pGfx->GetDeviceContext());
	}

	pPixelShader = new PixelShader(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), path);
}

#pragma endregion
