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

	//Creates a texture based off the designated path and adds it to the textures vector
	void CreateTexture(const wchar_t* path);

	void SetPosition(float x, float y, float z) {
		_pTransform->SetPosition(x, y, z);
	}

	void SetRotation(float x, float y, float z) {
		_pTransform->SetRotation(x, y, z);
	}

	//Setters
	//Set Shader function calls the loading logic for the shader
	void SetShader(WCHAR* path);

	Material& GetMaterial();
	
private:
	void Initialize();

	
protected:
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;
	VertexBuffer* pVertexBuffer;
	IndexBuffer* pIndexBuffer;
	
	Graphics* _pGfx;

	std::vector<ID3D11ShaderResourceView*> pTextures;

	bool mHasTextures = false;

	Material mMaterial;
};

