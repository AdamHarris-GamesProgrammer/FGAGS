#pragma once

#include "Bindable.h"

/// <summary>
/// Index Buffer class is used to create and bind a index buffer to the graphical pipeline 
/// </summary>
/// //B1
class IndexBuffer : public Bindable {
public:
	//Creates the Index Buffer
	IndexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, std::vector<unsigned short>& indices)
		: Bindable(device, context), _indexCount(indices.size())
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short) * indices.size();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indices.data();
		_pDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	}

	~IndexBuffer() {
		if (_pIndexBuffer)_pIndexBuffer->Release();

		_pIndexBuffer = nullptr;
	}

	//Overrides the base Bind method so the index buffer can be bound to the pipeline
	void Bind() override
	{
		_pDeviceContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}

	//Returns the index count, needed for rendering the object
	UINT GetIndexCount() {
		return _indexCount;
	}

private:
	//Stores the index buffer and the index count 
	ID3D11Buffer* _pIndexBuffer;
	UINT _indexCount;
};
