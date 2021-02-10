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

//K1
class GameObject
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



	//Setters
	//Set Shader function calls the loading logic for the shader
	void SetShader(WCHAR* path);


	void SetName(std::string& name);

	//Initializes the bounding sphere with the radius of the model
	virtual void InitializeBoundingSphere();

	Material& GetMaterial();
	Transform& GetTransform();
	//Particle* GetParticle() const { return mParticle; }
	Rigidbody* GetBody() { return pRigidBody; }
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
	Transform mTransform;
	//Particle* mParticle;
	Rigidbody* pRigidBody;

};

