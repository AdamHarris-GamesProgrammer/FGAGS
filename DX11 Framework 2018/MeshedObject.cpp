#include "MeshedObject.h"

//Loads the object immediately if a filepath is given
MeshedObject::MeshedObject(Graphics* gfx, const char* filepath)
	: GameObject(gfx)
{
	Load(filepath);
	_name = "MeshedObject";
}

MeshedObject::MeshedObject(Graphics* gfx) : GameObject(gfx) {}


void MeshedObject::Load(const char* filepath)
{
	//Loads the mesh data
	mMesh = OBJLoader::Load(filepath, _pGfx->GetDevice(),_pGfx->GetDeviceContext(),false);

	//Sets the vertex and index buffers
	pVertexBuffer = new VertexBuffer(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), mMesh.Vertices);
	pIndexBuffer = new IndexBuffer(_pGfx->GetDevice(), _pGfx->GetDeviceContext(), mMesh.Indices);

	//Clears the vertices and indices array in order to reduce memory overhead
	mMesh.Vertices.clear();
	mMesh.Indices.clear();

}

void MeshedObject::Load(std::string& filepath)
{
	Load(filepath.c_str());
}

