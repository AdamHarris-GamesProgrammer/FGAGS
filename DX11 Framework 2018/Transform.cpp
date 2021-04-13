#include "Transform.h"

void TransformComponent::Update(float dt)
{
	//Loads the transform matrix

	_orientation.Normalize();

	//Calculate the transform matrix with position and orientation
	_transform.SetOrientationAndPosition(_orientation, _position);

	float transform[16];
	_transform.DirectXArray(transform);
	SetDirectXTransform(XMFLOAT4X4(transform));

	//Perform the scaling operation
	XMMATRIX transformMatrix = XMLoadFloat4x4(&_DirectXTransform);

	//Sets the scale, position and rotation matrices
	XMMATRIX objectScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

	//Calculates the transform
	XMMATRIX calculatedTransform = XMMatrixMultiply(transformMatrix, objectScale);

	//Stores the transform
	XMStoreFloat4x4(&_DirectXTransform, calculatedTransform);
}

DirectX::XMFLOAT4X4 TransformComponent::GetDirectXTransform() const
{
	return _DirectXTransform;
}

Vector3 TransformComponent::GetPosition() const
{
	return _position;
}

Vector3 TransformComponent::GetRotation() const
{
	return _rotation;
}

Vector3 TransformComponent::GetScale() const
{
	return _scale;
}

void TransformComponent::SetDirectXTransform(XMFLOAT4X4 transform)
{
	_DirectXTransform = transform;
}

void TransformComponent::SetPosition(Vector3 position)
{
	_position = position;
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	_position.Set(x, y, z);
}

void TransformComponent::SetPosition(XMFLOAT3 position)
{
	_position.Set(position.x, position.y, position.z);
}

void TransformComponent::SetRotation(Vector3 rotation)
{
	_rotation = rotation;
	_orientation._i = rotation.x;
	_orientation._j = rotation.y;
	_orientation._k = rotation.z;
}

void TransformComponent::SetRotation(float x, float y, float z)
{
	_rotation.Set(x, y, z);
	_orientation._i = x;
	_orientation._j = y;
	_orientation._k = z;

}

void TransformComponent::SetScale(float x, float y, float z)
{
	_scale.Set(x, y, z);
}

void TransformComponent::SetScale(Vector3 scale)
{
	_scale = scale;
}
