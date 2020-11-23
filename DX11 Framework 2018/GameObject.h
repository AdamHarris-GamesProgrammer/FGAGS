#pragma once
#include "OBJLoader.h"
#include "Graphics.h"
#include <vector>
#include "DDSTextureLoader.h"
#include "Shaders.h"


class GameObject
{
public:
	GameObject();
	GameObject(Graphics* gfx);

	void Update(float dt);

	XMMATRIX CalculateTransform();

	virtual void Draw();


	void CreateTexture(const wchar_t* path);

	void SetName(std::string& name);
	std::string GetName();


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

	void Initialize();

	
protected:
	bool hasTextures = false;

	std::string mName = "Gameobject";

	Shaders* mShader;

	Graphics* mGfx;

	std::vector<ID3D11ShaderResourceView*> mTextures;

	//Transform Properties
	XMFLOAT4X4 mTransform;
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;

};

