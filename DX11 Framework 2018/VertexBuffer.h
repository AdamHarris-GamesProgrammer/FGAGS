#pragma once
#include "Structs.h"
#include <d3d11.h>
#include <vector>
#include "Graphics.h"

class VertexBuffer
{
public:
	VertexBuffer(Graphics* gfxIn);
	//VertexBuffer(Graphics* gfxIn, std::vector<SimpleVertex>& vertices);

	void AddElement(const SimpleVertex& vertex);
	
	ID3D11Buffer* GetBuffer() const;
	void SetBuffer();
	void Finalise();

private:
	std::vector<SimpleVertex>* vertexBuffer;
	ID3D11Buffer* pVertexBuffer;

	Graphics* gfx;

	//ID3D11Device* pDevice;
	//ID3D11DeviceContext* pDeviceContext;

};

