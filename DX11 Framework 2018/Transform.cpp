#include "Transform.h"

void TransformComponent::Update(float dt)
{
	//Loads the transform matrix
	XMMATRIX transformMatrix = XMLoadFloat4x4(&mTransform);

	//Sets the scale, position and rotation matrices
	XMMATRIX objectScale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	XMMATRIX objectPosition = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	XMMATRIX objectRotation = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);

	//Calculates the transform
	XMMATRIX calculatedTransform = XMMatrixMultiply(transformMatrix, objectScale);

	//Stores the transform
	XMStoreFloat4x4(&mTransform, calculatedTransform);
}

DirectX::XMFLOAT4X4 TransformComponent::GetTransform() const
{
	return mTransform;
}

Vector3 TransformComponent::GetPosition() const
{
	return mPosition;
}

Vector3 TransformComponent::GetRotation() const
{
	return mRotation;
}

Vector3 TransformComponent::GetScale() const
{
	return mScale;
}

void TransformComponent::SetTransform(XMFLOAT4X4 transform)
{
	mTransform = transform;
}

void TransformComponent::SetPosition(Vector3 position)
{
	mPosition = position;
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	mPosition.Set(x, y, z);
}

void TransformComponent::SetPosition(XMFLOAT3 position)
{
	mPosition.Set(position.x, position.y, position.z);
}

void TransformComponent::SetRotation(Vector3 rotation)
{
	mRotation = rotation;
}

void TransformComponent::SetRotation(float x, float y, float z)
{
	mRotation.Set(x, y, z);

}

void TransformComponent::SetScale(float x, float y, float z)
{
	mScale.Set(x, y, z);
}

void TransformComponent::SetScale(Vector3 scale)
{
	mScale = scale;
}
