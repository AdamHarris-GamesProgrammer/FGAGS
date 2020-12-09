#pragma once
#include "OBJLoader.h"
#include "Graphics.h"
#include <vector>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "BindableHeaders.h"

class GameObject
{
public:
	GameObject();
	GameObject(Graphics* gfx);

	XMMATRIX CalculateTransform();

	virtual void Draw();
	virtual void Update(float dt);
	virtual bool TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection);


	void CreateTexture(const wchar_t* path);

	void SetName(std::string& name);
	std::string GetName();

	void SetShader(WCHAR* path);

	void SetTransform(XMMATRIX transform);
	void SetTransform(XMFLOAT4X4 transform);
	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);
	void SetMaterialDiffuse(XMFLOAT4 color);
	void SetMaterialAmbient(XMFLOAT4 color);
	void SetMaterialSpecular(XMFLOAT4 color);

	XMFLOAT4X4 GetTransform() const;
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;
	XMFLOAT3 GetScale() const;
	Material GetMaterial() const;



private:

	void Initialize();

	
protected:
	bool hasTextures = false;

	std::string mName = "Gameobject";

	VertexShader* mVertexShader;
	PixelShader* mPixelShader;
	VertexBuffer* mVertexBuffer;
	IndexBuffer* mIndexBuffer;

	BoundingSphere mBoundingSphere;

	Graphics* mGfx;

	std::vector<ID3D11ShaderResourceView*> mTextures;

	Material mMaterial;

	//Transform Properties
	XMFLOAT4X4 mTransform;
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;

};

