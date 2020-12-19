#pragma once
#include "Bindable.h"
#include <algorithm>

/// <summary>
/// Vertex Buffer class is used to create and bind a Vertex Buffer to the graphical pipeline 
/// </summary>
/// //B1
class VertexBuffer : public Bindable {
public:
	//Creates a Vertex Buffer object
	VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<SimpleVertex>& vertices) : Bindable(device, context) {
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices.data();

		pDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	}

	~VertexBuffer() {
		if (pVertexBuffer)pVertexBuffer->Release();
	}

	//Overrides the base Bind function  to set the Vertex Buffer for the current object
	void Bind() override
	{
		pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &mStride, &mOffset);
	}

private:
	ID3D11Buffer* pVertexBuffer = nullptr;

	UINT mStride = sizeof(SimpleVertex);
	UINT mOffset = 0;
};