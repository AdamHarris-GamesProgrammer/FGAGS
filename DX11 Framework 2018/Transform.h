#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vector3.h"
#include "Component.h"
#include "ComponentIDs.h"

using namespace DirectX;

class TransformComponent : public Component
{
public:


	TransformComponent(Object* owner) : Component(owner, ComponentID::Transform, 80.0f) {
		XMStoreFloat4x4(&mTransform, XMMatrixIdentity());
		mPosition = Vector3(0.0f, 0.0f, 0.0f);
		mRotation = Vector3(0.0f, 0.0f, 0.0f);
		mScale = Vector3(1.0f, 1.0f, 1.0f);
	}

	void Update(float dt) override;

	XMFLOAT4X4 GetTransform() const;
	Vector3 GetPosition() const;

	Vector3 GetRotation() const;
	Vector3 GetScale() const;
	void SetTransform(XMFLOAT4X4 transform);
	void SetPosition(Vector3 position);
	void SetPosition(XMFLOAT3 position);
	void SetRotation(Vector3 rotation);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetScale(Vector3 scale);

private:
	//Transform Properties
	XMFLOAT4X4 mTransform;
	Vector3 mPosition;
	Vector3 mRotation;
	Vector3 mScale;

};

