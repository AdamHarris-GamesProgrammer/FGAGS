#pragma once
#include "OBJLoader.h"
#include "Graphics.h"

class GameObject
{
public:
	GameObject();
	GameObject(Graphics* gfx, char* filepath);

	

	void Update(float dt);
	void Draw(Graphics* gfx);

	XMFLOAT4X4 GetTransform();

	void SetTransform(XMMATRIX transform);
	void SetTransform(XMFLOAT4X4 transform);
	void SetPosition(XMFLOAT4 position);
	void SetRotation(XMFLOAT4 rotation);
	void SetScale(XMFLOAT4 scale);

private:
	void Initialize();

protected:
	//Mesh
	MeshData mMesh;

	//Transform Properties
	XMFLOAT4X4 mTransform;
	XMFLOAT4 mPosition;
	XMFLOAT4 mRotation;
	XMFLOAT4 mScale;

};

