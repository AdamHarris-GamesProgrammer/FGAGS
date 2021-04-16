#pragma once
#include "Component.h"
#include "Graphics.h"
#include "BindableHeaders.h"
#include <vector>

class RendererComponent : public Component
{
public:
	RendererComponent(Object* owner, std::shared_ptr<Graphics> gfx);

	void Update(float dt) override;

	//Creates a texture based on the provided path
	void CreateTexture(const wchar_t* path);

	//Sets the shader for the rendered object
	void SetShader(WCHAR* path);

	//Returns the material
	Material& GetMaterial();

	//Loads the model
	void Load(const char* filepath);
	void Load(std::string& filepath);

	//Creates the vertex and index buffer
	void CreateVertexBuffer(std::vector<SimpleVertex>& vertices);
	void CreateIndexBuffer(std::vector<unsigned short>& indices);

private:
	std::unique_ptr<IndexBuffer> _pIndexBuffer;
	std::vector<std::unique_ptr<Bindable>> _pBindables;

	std::shared_ptr<Graphics> _pGfx;

	std::vector<ID3D11ShaderResourceView*> _pTextures;

	bool _hasTextures = false;

	Material _material;

	MeshData _mesh;
};

