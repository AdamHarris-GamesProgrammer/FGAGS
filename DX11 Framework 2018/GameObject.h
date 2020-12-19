#pragma once
#include "OBJLoader.h"
#include "Graphics.h"
#include <vector>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "BindableHeaders.h"

//K1
class GameObject
{
public:
	GameObject();
	GameObject(Graphics* gfx);
	~GameObject();

	//Calculates the objects transform 
	void CalculateTransform();

	//Allows the object to be drawn
	virtual void Draw();

	//Update method for the object
	virtual void Update(float dt);

	//TestCollision with a ray, used for picking 
	virtual bool TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection);

	//Creates a texture based off the designated path and adds it to the textures vector
	void CreateTexture(const wchar_t* path);



	//Setters
	//Set Shader function calls the loading logic for the shader
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
	void SetName(std::string& name);


	//Getters 
	XMFLOAT4X4 GetTransform() const;
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;
	XMFLOAT3 GetScale() const;
	Material GetMaterial() const;
	std::string GetName() const;




private:
	void Initialize();

	
protected:


	VertexShader* pVertexShader;
	PixelShader* pPixelShader;
	VertexBuffer* pVertexBuffer;
	IndexBuffer* pIndexBuffer;

	BoundingSphere mBoundingSphere;

	Graphics* pGfx;

	std::vector<ID3D11ShaderResourceView*> pTextures;

	bool mHasTextures = false;

	std::string mName = "Gameobject";

	Material mMaterial;

	//Transform Properties
	XMFLOAT4X4 mTransform;
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;

};

