#pragma once
#include "Component.h"
#include "Graphics.h"
#include "BindableHeaders.h"
#include <vector>

class RendererComponent : public Component
{
public:
	RendererComponent(Object* owner, Graphics* gfx);

	void Update(float dt) override;

	void CreateTexture(const wchar_t* path);

	void SetShader(WCHAR* path);

	Material& GetMaterial();

	void Load(const char* filepath);
	void Load(std::string& filepath);


	void CreateVertexBuffer(std::vector<SimpleVertex>& vertices);
	void CreateIndexBuffer(std::vector<unsigned short>& indices);

private:
	std::unique_ptr<IndexBuffer> pIndexBuffer;
	std::vector<std::unique_ptr<Bindable>> _pBindables;

	Graphics* _pGfx;

	std::vector<ID3D11ShaderResourceView*> pTextures;

	bool mHasTextures = false;

	Material mMaterial;

	MeshData mMesh;
};

