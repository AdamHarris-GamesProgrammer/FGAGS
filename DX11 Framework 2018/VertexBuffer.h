#pragma once
#include "Bindable.h"
#include <algorithm>

class VertexBuffer : public Bindable {
public:
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

		mDevice->CreateBuffer(&bd, &InitData, &vertexBuffer);
	}

	void Bind() override
	{
		mContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	}

private:
	ID3D11Buffer* vertexBuffer = nullptr;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
};