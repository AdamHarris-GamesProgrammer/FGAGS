#pragma once
#include "OBJLoader.h"
#include "Graphics.h"
#include <vector>
#include "DDSTextureLoader.h"

class GameObject
{
public:
	GameObject();
	GameObject(Graphics* gfx, char* filepath);

	void Update(float dt);

	XMMATRIX CalculateTransform();

	void Draw();


	void CreateTexture(wchar_t* path);

	void SetTransform(XMMATRIX transform);
	void SetTransform(XMFLOAT4X4 transform);
	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);

	XMFLOAT4X4 GetTransform();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();

private:
	bool hasTextures = false;

	void Initialize();

	Graphics* gfx;
protected:
	//Mesh
	MeshData mMesh;


	std::vector<ID3D11ShaderResourceView*> mTextures;

	//Transform Properties
	XMFLOAT4X4 mTransform;
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;

};

