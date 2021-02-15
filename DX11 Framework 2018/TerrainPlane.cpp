
#include "TerrainPlane.h"

#include <fstream>
#include <vector>

void TerrainPlane::Make(float width, float depth, UINT m, UINT n, const char* heightmap)
{
	//Calculates the amount of vertices and faces required for the plane
	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;

	//Creates vectors for both of the vertices and the indices
	std::vector<SimpleVertex> Vertices;
	std::vector<WORD> Indices;

	//Calculates half of the width and depth
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	//Calculates the delta between each row
	float dx = width / (n - 1);
	//Calculates the delta between each column
	float dz = depth / (m - 1);

	//Calculates the delta between each row and column of tex coords
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	//Resizes the vertices vector reducing overhead from not calling copy constructor
	Vertices.resize(vertexCount);

	float* arr = ReadHeightMap(m, n, heightmap);

	//Loops through the amount of rows
	for (UINT i = 0; i < m; ++i)
	{
		//Calculates the z position value
		float z = halfDepth - i * dz;
		//Loops through the amount of columns
		for (UINT j = 0; j < n; ++j)
		{
			//Calculates the X position value
			float x = -halfWidth + j * dx;

			float yVal = arr[i * n + j];

			//Sets the normals and positions of the vertex
			Vertices[i * n + j].Pos = XMFLOAT3(x, yVal, z);
			Vertices[i * n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			// Stretch texture over grid.
			Vertices[i * n + j].TexC.x = j * du;
			Vertices[i * n + j].TexC.y = i * dv;
		}
	}

	pVertexBuffer = new VertexBuffer(pGfx->GetDevice(), pGfx->GetDeviceContext(), Vertices);

	//Resizes the indices vector to avoid calling the copy constructor for every new index
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
	//Creates the index buffer
	pIndexBuffer = new IndexBuffer(pGfx->GetDevice(), pGfx->GetDeviceContext(), Indices);

	//Sets the shader
	SetShader(L"PhongDif.fx");
}

float* TerrainPlane::ReadHeightMap(UINT m, UINT n, const char* heightmap)
{
	std::vector<unsigned char> in(m * n);

	std::ifstream inFile;
	inFile.open(heightmap, std::ios_base::binary);

	if (inFile) {
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		inFile.close();
	}

	float* heightMapArr = new float[m * n];
	//TODO replace 3.0 with controllable scale
	for (UINT i = 0; i < m * n; ++i) {
		heightMapArr[i] = (in[i] / 255.0f) * 1.0f;
	}

	return heightMapArr;
}
