#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vector3.h"
#include "Component.h"
#include "ComponentIDs.h"
#include "Quaternion.h"
#include "Matrix4.h"

using namespace DirectX;

class TransformComponent : public Component
{
public:


	TransformComponent(Object* owner) : Component(owner, ComponentID::Transform, 80.0f) {
		XMStoreFloat4x4(&_DirectXTransform, XMMatrixIdentity());
		_position = Vector3(0.0f, 0.0f, 0.0f);
		_rotation = Vector3(0.0f, 0.0f, 0.0f);
		_scale = Vector3(1.0f, 1.0f, 1.0f);
	}

	void Update(float dt) override;

	XMFLOAT4X4 GetDirectXTransform() const;
	void SetDirectXTransform(XMFLOAT4X4 transform);


	Vector3 GetPosition() const;

	Vector3 GetRotation() const;
	Vector3 GetScale() const;
	Quaternion GetOrientation() const {
		return _orientation;
	}

	void SetOrientation(Quaternion val) {
		_orientation = val;
	}
	
	void SetPosition(Vector3 position);
	void SetPosition(XMFLOAT3 position);
	void SetRotation(Vector3 rotation);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetScale(Vector3 scale);

private:
	//Transform Properties
	XMFLOAT4X4 _DirectXTransform;
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;
	Quaternion _orientation;
	Matrix4 _transform;
};

