#pragma once
#include "OBJLoader.h"
#include "Graphics.h"
#include <vector>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "BindableHeaders.h"
#include "Transform.h"
#include "Particle.h"
#include "Rigidbody.h"
#include "Object.h"

//K1
class GameObject : public Object
{
public:
	GameObject();
	GameObject(Graphics* gfx);
	~GameObject();

	//Allows the object to be drawn
	virtual void Draw();

	//Update method for the object
	virtual void Update(float dt);

	void UpdateTransforms();

	//TestCollision with a ray, used for picking 
	virtual bool TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection);

	//Creates a texture based off the designated path and adds it to the textures vector
	void CreateTexture(const wchar_t* path);

	void SetPosition(float x, float y, float z) {
		_transform.SetPosition(x, y, z);
		pRigidBody->SetPosition(x, y, z);
		pRigidBody->CalculateDerivedData();
	}

	void SetRotation(float x, float y, float z) {
		_transform.SetRotation(x, y, z);
		pRigidBody->SetRotation(x, y, z);
		pRigidBody->CalculateDerivedData();
	}

	//Setters
	//Set Shader function calls the loading logic for the shader
	void SetShader(WCHAR* path);


	//Initializes the bounding sphere with the radius of the model
	virtual void InitializeBoundingSphere();

	Material& GetMaterial();
	Rigidbody* GetBody() { return pRigidBody; }

	
private:
	void Initialize();

	
protected:
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;
	VertexBuffer* pVertexBuffer;
	IndexBuffer* pIndexBuffer;
	
	BoundingSphere mBoundingSphere;

	Graphics* _pGfx;

	std::vector<ID3D11ShaderResourceView*> pTextures;

	bool mHasTextures = false;


	Material mMaterial;
	Rigidbody* pRigidBody;

};

