#pragma once
#include <DirectXColors.h>
#include <d3d11.h>
#include "Structs.h"
using namespace DirectX;

class Cube {
public:
	Cube(ID3D11Device* device, ID3D11Buffer* vertices, ID3D11Buffer* indices)
	{
		pDevice = device;
		pVertices = vertices;
		pIndex = indices;

		XMStoreFloat4x4(&worldPosition, XMMatrixIdentity());
	}

	void Draw() {

	}

	void Update() {
		XMStoreFloat4x4(&worldPosition, transformMatrix);
	}

	XMFLOAT4X4 GetPosition() { return worldPosition; }

	void SetMatrix(XMMATRIX matrixIn) { transformMatrix = matrixIn; }

	void SetupVertices() {
		vertices =  new SimpleVertex[] {
			{ XMFLOAT3(-1.0f,-1.0f,-1.0f)	,XMFLOAT4(0.0f,0.0f,1.0f,1.0f)	},
			{ XMFLOAT3(1.0f,-1.0f,-1.0f)	,XMFLOAT4(0.0f,1.0f,0.0f,1.0f)	},
			{ XMFLOAT3(-1.0f,1.0f,-1.0f)	,XMFLOAT4(0.0f,1.0f,1.0f,1.0f)	},
			{ XMFLOAT3(1.0f,1.0f,-1.0f)		,XMFLOAT4(1.0f,0.0f,0.0f,1.0f)	},
			{ XMFLOAT3(-1.0f,-1.0f,1.0f)	,XMFLOAT4(1.0f,0.0f,1.0f,1.0f)	},
			{ XMFLOAT3(1.0f,-1.0f,1.0f)		,XMFLOAT4(1.0f,1.0f,0.0f,1.0f)	},
			{ XMFLOAT3(-1.0f,1.0f,1.0f)		,XMFLOAT4(1.0f,1.0f,1.0f,1.0f)	},
			{ XMFLOAT3(1.0f,1.0f,1.0f)		,XMFLOAT4(0.0f,0.0f,0.0f,1.0f)	}
		};

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;


		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;

		pDevice->CreateBuffer(&bd, &InitData, &pVertices);
	}
	void SetupIndices() {
		indices = new unsigned short[]{
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};


		// Create index buffer
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(indices);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = indices;
		pDevice->CreateBuffer(&bd, &InitData, &pIndex);
	}

private:
	XMFLOAT4X4 worldPosition;

	unsigned short* indices;

	SimpleVertex* vertices;
	

	ID3D11Device* pDevice;
	ID3D11Buffer* pVertices;
	ID3D11Buffer* pIndex;
	XMMATRIX transformMatrix;
};

