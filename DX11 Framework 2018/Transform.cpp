#include "Transform.h"

void Transform::Update()
{
		//Loads the transform matrix
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);

	//Sets the scale, position and rotation matrices
	XMMATRIX objectScale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	XMMATRIX objectPosition = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	XMMATRIX objectRotation = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);

	//Calculates the transform
	XMMATRIX calculatedTransform = XMMatrixMultiply(objectScale, objectPosition) * objectRotation;

	//Stores the transform
	XMStoreFloat4x4(&mTransform, calculatedTransform);
}

DirectX::XMFLOAT4X4 Transform::GetTransform() const
{
	return mTransform;
}

DirectX::XMFLOAT3 Transform::GetPosition() const
{
	return mPosition;
}

DirectX::XMFLOAT3 Transform::GetRotation() const
{
	return mRotation;
}

DirectX::XMFLOAT3 Transform::GetScale() const
{
	return mScale;
}

void Transform::SetTransform(XMFLOAT4X4 transform)
{
	mTransform = transform;
}

void Transform::SetPosition(XMFLOAT3 position)
{
	mPosition = position;
}

void Transform::SetPosition(float x, float y, float z)
{
	mPosition = XMFLOAT3(x, y, z);
}

void Transform::SetRotation(XMFLOAT3 rotation)
{
	mRotation = rotation;
}

void Transform::SetRotation(float x, float y, float z)
{
	mRotation = XMFLOAT3(x, y, z);
}

void Transform::SetScale(float x, float y, float z)
{
	mScale = XMFLOAT3(x, y, z);
}

void Transform::SetScale(XMFLOAT3 scale)
{
	mScale = scale;
}
