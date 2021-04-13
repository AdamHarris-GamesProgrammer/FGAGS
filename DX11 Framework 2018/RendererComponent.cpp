#include "RendererComponent.h"

RendererComponent::RendererComponent(Object* owner, Graphics* gfx) :
	Component(owner, ComponentID::Renderer, 150.0f), _pGfx(gfx)
{
	//Reserve 4 slots for bindables
	_pBindables.reserve(3);

	_pBindables.emplace_back(std::make_unique<VertexShader>(_pGfx->GetDevice(), _pGfx->GetDeviceContext()));
}

void RendererComponent::Update(float dt)
{
	//Binds all bindables
	for (auto& bindable : _pBindables) {
		bindable->Bind();
	}
	_pIndexBuffer->Bind();

	//Sets the textures for the shaders
	if (_hasTextures) {
		_pGfx->GetDeviceContext()->PSSetShaderResources(0, _pTextures.size(), &_pTextures[0]);
	}

	//Sets the constant buffer
	_pGfx->SetConstantBuffer();

	//Sends the objects material and transform to the graphics class for the updated constant buffer
	_pGfx->SetObjectBuffers(_material, _pOwner->GetTransform().GetDirectXTransform());

	//Draws the object
	_pGfx->Draw(_pIndexBuffer->GetIndexCount());
}

void RendererComponent::CreateTexture(const wchar_t* path)
{
	ID3D11ShaderResourceView* texture;

	_hasTextures = true;

	//Creates and adds the texture to the texture vector
	CreateDDSTextureFromFile(_pGfx->GetDevice(), path, nullptr, &texture);
	_pTextures.push_back(texture);
}

void RendererComponent::SetShader(WCHAR* path)
{
	//Creates a new pixel shader object and places it at the back of our bindables 
	_pBindables.emplace_back(std::make_unique<PixelShader>(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), path));
}

Material& RendererComponent::GetMaterial()
{
	return _material;
}

void RendererComponent::Load(const char* filepath)
{
	//Loads the mesh data
	_mesh = OBJLoader::Load(filepath, _pGfx->GetDevice(), _pGfx->GetDeviceContext(), false);

	//Sets the vertex and index buffers
	CreateVertexBuffer(_mesh.Vertices);
	CreateIndexBuffer(_mesh.Indices);

	//Clears the vertices and indices array in order to reduce memory overhead
	_mesh.Vertices.clear();
	_mesh.Indices.clear();
}

void RendererComponent::Load(std::string& filepath)
{
	Load(filepath.c_str());
}

void RendererComponent::CreateVertexBuffer(std::vector<SimpleVertex>& vertices)
{
	//Creates a vertex buffer object and adds it to our bindables system 
	_pBindables.emplace_back(std::make_unique<VertexBuffer>(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), vertices));
}

void RendererComponent::CreateIndexBuffer(std::vector<unsigned short>& indices)
{
	//Creates a index buffer object and adds it to our bindables system 
	_pIndexBuffer = std::make_unique<IndexBuffer>(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), indices);
}
