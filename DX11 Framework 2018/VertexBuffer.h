#pragma once
#include "Structs.h"
#include <d3d11.h>
#include <vector>
#include "GfxResources.h"

class VertexBuffer : public GfxResources
{
public:
	VertexBuffer() {}
	VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context);
	//VertexBuffer(Graphics* gfxIn, std::vector<SimpleVertex>& vertices);

	void AddElement(const SimpleVertex& vertex);
	
	ID3D11Buffer* GetBuffer() const;
	void SetBuffer(Graphics& gfx);
	void Finalise(Graphics& gfx);

private:
	std::vector<SimpleVertex>* vertexBuffer;
	ID3D11Buffer* pVertexBuffer;

	//Graphics* gfx;

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

};

