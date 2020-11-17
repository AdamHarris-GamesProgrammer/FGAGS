#pragma once
#include "GameObject.h"

class Plane : public GameObject {
public:
	Plane(Graphics* gfx) : GameObject(gfx) {
	}

	void Make(float width, float depth, UINT m, UINT n) {
		UINT vertexCount = m * n;
		UINT faceCount = (m - 1) * (n - 1) * 2;

		std::vector<SimpleVertex> Vertices;
		std::vector<WORD> Indices;

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		Vertices.resize(vertexCount);
		for (UINT i = 0; i < m; ++i)
		{
			float z = halfDepth - i * dz;
			for (UINT j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * dx;

				Vertices[i * n + j].Pos = XMFLOAT3(x, 0.0f, z);
				Vertices[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

				// Stretch texture over grid.
				Vertices[i * n + j].TexC.x = j * du;
				Vertices[i * n + j].TexC.y = i * dv;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(SimpleVertex) * Vertices.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &Vertices[0];

		mGfx->GetDevice()->CreateBuffer(&bd, &InitData, &mVertexBuffer);


		Indices.resize(faceCount * 3); // 3 indices per face

		// Iterate over each quad and compute indices.
		UINT k = 0;
		for (UINT i = 0; i < m - 1; ++i)
		{
			for (UINT j = 0; j < n - 1; ++j)
			{
				Indices[k] = i * n + j;
				Indices[k + 1] = i * n + j + 1;
				Indices[k + 2] = (i + 1) * n + j;

				Indices[k + 3] = (i + 1) * n + j;
				Indices[k + 4] = i * n + j + 1;
				Indices[k + 5] = (i + 1) * n + j + 1;

				k += 6; // next quad
			}
		}

		indexCount = Indices.size();

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * Indices.size();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = &Indices[0];
		mGfx->GetDevice()->CreateBuffer(&bd, &InitData, &mIndexBuffer);


	}

	void Draw() override {
		mShader->BindShaders();
		mGfx->BindTextures(0, mTextures.size(), mTextures);
		mGfx->UpdateBuffers(mTransform);
		mGfx->SwitchIndexBuffer(mIndexBuffer);
		mGfx->SwitchVertexBuffer(mVertexBuffer);
		mGfx->Draw(indexCount);
	}

	ID3D11Buffer* GetVertexBuffer() {
		return mVertexBuffer;
	}


	ID3D11Buffer* GetIndexBuffer() {
		return mIndexBuffer;
	}

	UINT GetIndexCount() {
		return indexCount;
	}

private:
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Device* mDevice;
	UINT indexCount = 0;
};
