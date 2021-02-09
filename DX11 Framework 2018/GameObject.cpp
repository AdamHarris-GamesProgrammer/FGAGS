#include "GameObject.h"

GameObject::GameObject()
{
	Initialize();
}


GameObject::GameObject(Graphics* gfx)
{
	this->pGfx = gfx;

	//Initializes the position and rotation to world origin
	mTransform = Transform();
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
	tensor.SetBlockInertiaTensor(Vector3(1.0,1.0,1.0), 5.0);
	pRigidBody->SetInertiaTensor(tensor);

	pRigidBody->ClearAccumulators();
	pRigidBody->CalculateDerivedData();


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
}

void GameObject::Update(float dt)
{
	pRigidBody->SetPosition(mTransform.GetPosition());
	pRigidBody->Update(dt);

	
	mTransform.SetPosition(pRigidBody->GetPosition());

	//Updates transform
	mTransform.Update();

	//Updates bounding sphere location
	mBoundingSphere.Center = mTransform.GetPosition();
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
	pGfx->SetObjectBuffers(mMaterial, mTransform.GetTransform());

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
	mBoundingSphere.Center = mTransform.GetPosition();


	//Sets the vertex shader
	pVertexShader = new VertexShader(pGfx->GetDevice(), pGfx->GetDeviceContext());
}

#pragma region Getters
std::string GameObject::GetName() const
{
	return mName;
}
Material& GameObject::GetMaterial() 
{
	return mMaterial;
}

Transform& GameObject::GetTransform() {
	return mTransform;
}

#pragma endregion

#pragma region Setters
void GameObject::SetShader(WCHAR* path)
{
	pPixelShader = new PixelShader(pGfx->GetDevice(), pGfx->GetDeviceContext(), path);
}
void GameObject::SetName(std::string& name)
{
	mName = name;
}

void GameObject::InitializeBoundingSphere()
{
	mBoundingSphere.Radius = 1.0f;
}

#pragma endregion
