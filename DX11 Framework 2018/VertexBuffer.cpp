#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics* gfxIn) : gfx(gfxIn)
{
	//pDevice = device;
	//pDeviceContext = deviceContext;
	vertexBuffer = new std::vector<SimpleVertex>();
	pVertexBuffer = nullptr;

	//gfx = gfxIn;
}

//VertexBuffer::VertexBuffer(Graphics* gfxIn, std::vector<SimpleVertex>& vertices) : gfx(gfxIn)
//{
//	//pDevice = device;
//	//pDeviceContext = deviceContext;
//	vertexBuffer = &vertices;
//	vertexBuffer = new std::vector<SimpleVertex>();
//	pVertexBuffer = nullptr;
//
//	//gfx = gfxIn;
//}

void VertexBuffer::AddElement(const SimpleVertex& vertex)
{
	vertexBuffer->push_back(vertex);
}

ID3D11Buffer* VertexBuffer::GetBuffer() const
{
	return pVertexBuffer;
}

void VertexBuffer::SetBuffer()
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	gfx->GetContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
}

void VertexBuffer::Finalise()
{
	SimpleVertex* vertices =  new SimpleVertex[vertexBuffer->size()];

	for (int i = 0; i < vertexBuffer->size(); i++)
	{
		vertices[i] = vertexBuffer->at(i);
	}
	

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	gfx->GetDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);

}
