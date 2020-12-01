#include "Plane.h"

void Plane::Make(float width, float depth, UINT m, UINT n)
{
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

	vBuffer = new VertexBuffer(mGfx->GetDevice(), mGfx->GetDeviceContext(), Vertices);


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

	iBuffer = new IndexBuffer(mGfx->GetDevice(), mGfx->GetDeviceContext(), Indices);

	PhongDifShader();
}

void Plane::Draw()
{
	mShader->BindShaders();
	vBuffer->Bind();
	iBuffer->Bind();

	if (hasTextures) {
		mGfx->GetDeviceContext()->PSSetShaderResources(0, mTextures.size(), &mTextures[0]);
	}

	mGfx->UpdateBuffers(mMaterial, mTransform);
	mGfx->Draw(iBuffer->GetIndexCount());
}
