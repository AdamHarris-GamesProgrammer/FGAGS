#pragma once
#include "Component.h"
#include "Graphics.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

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
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;
	VertexBuffer* pVertexBuffer;
	IndexBuffer* pIndexBuffer;

	Graphics* _pGfx;

	std::vector<ID3D11ShaderResourceView*> pTextures;

	bool mHasTextures = false;

	Material mMaterial;

	MeshData mMesh;
};

