#include "GameObject.h"

GameObject::GameObject() 
{
	Initialize();
}



GameObject::GameObject(Graphics* gfx)
{
	this->pGfx = gfx;
	//Initializes the position and rotation to world origin
	pRigidBody = new Rigidbody();

	//TODO Remove temporary Rigidbody cube code
	pRigidBody->SetMass(5.0);
	pRigidBody->SetOrientation(1, 0, 0, 0);
	pRigidBody->SetRotation(0.0, 0.0, 0.0);
	pRigidBody->SetCanSleep(true);
	pRigidBody->SetAwake(false);
	pRigidBody->SetAngularDamping(0.8f);
	pRigidBody->SetLinearDamping(0.95f);
	pRigidBody->SetVelocity(0, 0, 0);
	pRigidBody->SetAcceleration(0.0, -10.0, 0);

	Matrix3 tensor;

	float coeff = 0.4 * pRigidBody->GetMass() * 1.0 * 1.0;
	tensor.SetInertiaTensorCoeffs(coeff, coeff, coeff);
	tensor.SetBlockInertiaTensor(Vector3(1.0, 1.0, 1.0), 5.0);
	pRigidBody->SetInertiaTensor(tensor);

	pRigidBody->ClearAccumulator();
	pRigidBody->CalculateDerivedData();

	_name = "GameObject";

	Initialize();
}

GameObject::~GameObject()
{
	delete pVertexShader;
	delete pPixelShader;
	delete pVertexBuffer;
	delete pIndexBuffer;

	pVertexBuffer = nullptr;
	pVertexShader = nullptr;
	pPixelShader = nullptr;
	pIndexBuffer = nullptr;

	pTextures.clear();
	

	delete pRigidBody;
	pRigidBody = nullptr;
}

void GameObject::Update(float dt)
{
	pRigidBody->SetPosition(_transform.GetPosition());
	
	pRigidBody->Update(dt);


	//Updates bounding sphere location
	mBoundingSphere.Center = (XMFLOAT3)_transform.GetPosition();
}

void GameObject::UpdateTransforms()
{
	_transform.SetPosition(pRigidBody->GetPosition());
	_transform.SetRotation(pRigidBody->GetOrientation().Identity());


	float transform[16];
	pRigidBody->GetTransform().DirectXArray(transform);

	_transform.SetTransform(XMFLOAT4X4(transform));

	//Updates transform
	_transform.Update();
}

bool GameObject::TestCollision(XMFLOAT4 rayOrigin, XMFLOAT4 rayDirection)
{
	return false;
}


void GameObject::Draw()
{
	//Binds the shaders and buffers
	pVertexShader->Bind();
	pPixelShader->Bind();
	pVertexBuffer->Bind();
	pIndexBuffer->Bind();

	//Sets the textures for the shaders
	if (mHasTextures) {
		pGfx->GetDeviceContext()->PSSetShaderResources(0, pTextures.size(), &pTextures[0]);
	}

	//Sets the constant buffer
	pGfx->SetConstantBuffer();

	//Sends the objects material and transform to the graphics class for the updated constant buffer
	pGfx->SetObjectBuffers(mMaterial, _transform.GetTransform());

	//Draws the object
	pGfx->Draw(pIndexBuffer->GetIndexCount());
}


void GameObject::CreateTexture(const wchar_t* path)
{
	ID3D11ShaderResourceView* texture;

	mHasTextures = true;

	//Creates and adds the texture to the texture vector
	CreateDDSTextureFromFile(pGfx->GetDevice(), path, nullptr, &texture);
	pTextures.push_back(texture);
}


void GameObject::Initialize()
{
	//Sets the radius and position of the bounding sphere
	mBoundingSphere.Radius = 0.0f;
	mBoundingSphere.Center = (XMFLOAT3)_transform.GetPosition();


	//Sets the vertex shader
	pVertexShader = new VertexShader(pGfx->GetDevice(), pGfx->GetDeviceContext());
}

#pragma region Getters
Material& GameObject::GetMaterial() 
{
	return mMaterial;
}

#pragma endregion

#pragma region Setters
void GameObject::SetShader(WCHAR* path)
{
	pPixelShader = new PixelShader(pGfx->GetDevice(), pGfx->GetDeviceContext(), path);
}

void GameObject::InitializeBoundingSphere()
{
	mBoundingSphere.Radius = 1.0f;
}

#pragma endregion
