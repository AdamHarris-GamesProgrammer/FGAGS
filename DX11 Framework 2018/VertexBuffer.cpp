#include "VertexBuffer.h"
#include "GfxResources.cpp"
#include "GfxResources.h"

VertexBuffer::VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context)
{
	pDevice = device;
	pDeviceContext = context;
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

void VertexBuffer::SetBuffer(Graphics& gfx)
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	GetContext(gfx)->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
}

void VertexBuffer::Finalise(Graphics& gfx)
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
	GetDevice(gfx)->CreateBuffer(&bd, &InitData, &pVertexBuffer);

}
