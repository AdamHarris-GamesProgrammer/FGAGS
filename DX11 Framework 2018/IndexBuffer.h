#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable {
public:
	IndexBuffer(Graphics* gfx, std::vector<WORD>& indices) 
		: Bindable(gfx), mIndexCount(indices.size())
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * indices.size();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indices.data();
		mGfx->GetDevice()->CreateBuffer(&bd, &InitData, &mIndexBuffer);
	}

	void Bind() override
	{
		mGfx->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}

	UINT GetIndexCount() {
		return mIndexCount;
	}

private:
	ID3D11Buffer* mIndexBuffer;
	UINT mIndexCount;
};
