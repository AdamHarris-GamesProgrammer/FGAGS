#include "RendererComponent.h"

RendererComponent::RendererComponent(Object* owner, Graphics* gfx) :
	Component(owner, ComponentID::Renderer, 150.0f), _pGfx(gfx)
{
	pVertexShader = new VertexShader(_pGfx->GetDevice(), _pGfx->GetDeviceContext());
}

void RendererComponent::Update(float dt)
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
	_pGfx->SetObjectBuffers(mMaterial, _pOwner->GetTransform().GetDirectXTransform());

	//Draws the object
	_pGfx->Draw(pIndexBuffer->GetIndexCount());
}

void RendererComponent::CreateTexture(const wchar_t* path)
{
	ID3D11ShaderResourceView* texture;

	mHasTextures = true;

	//Creates and adds the texture to the texture vector
	CreateDDSTextureFromFile(_pGfx->GetDevice(), path, nullptr, &texture);
	pTextures.push_back(texture);
}

void RendererComponent::SetShader(WCHAR* path)
{
	if (pVertexShader == nullptr) {
		pVertexShader = new VertexShader(_pGfx->GetDevice(), _pGfx->GetDeviceContext());
	}

	pPixelShader = new PixelShader(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), path);
}

Material& RendererComponent::GetMaterial()
{
	return mMaterial;
}

void RendererComponent::Load(const char* filepath)
{
	//Loads the mesh data
	mMesh = OBJLoader::Load(filepath, _pGfx->GetDevice(), _pGfx->GetDeviceContext(), false);

	//Sets the vertex and index buffers
	CreateVertexBuffer(mMesh.Vertices);
	CreateIndexBuffer(mMesh.Indices);

	//Clears the vertices and indices array in order to reduce memory overhead
	mMesh.Vertices.clear();
	mMesh.Indices.clear();
}

void RendererComponent::Load(std::string& filepath)
{
	Load(filepath.c_str());
}

void RendererComponent::CreateVertexBuffer(std::vector<SimpleVertex>& vertices)
{
	pVertexBuffer = new VertexBuffer(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), vertices);
}

void RendererComponent::CreateIndexBuffer(std::vector<unsigned short>& indices)
{
	pIndexBuffer = new IndexBuffer(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), indices);
}
