#pragma once
#include "Bindable.h"
#include <algorithm>

class VertexBuffer : public Bindable {
public:
	VertexBuffer(Graphics* gfx, std::vector<SimpleVertex>& vertices) : Bindable(gfx) {
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * vertices.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices.data();

		mGfx->GetDevice()->CreateBuffer(&bd, &InitData, &vertexBuffer);
	}

	void Bind() override
	{
		mGfx->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	}

private:
	ID3D11Buffer* vertexBuffer = nullptr;
	std::vector<SimpleVertex>* Vertices;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
};