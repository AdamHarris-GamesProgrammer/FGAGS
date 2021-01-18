#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Transform
{
public:
	Transform() {
		XMStoreFloat4x4(&mTransform, XMMatrixIdentity());
		mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
		mRotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
		mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	}

	void Update();

	XMFLOAT4X4 GetTransform() const;
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;
	XMFLOAT3 GetScale() const;
	void SetTransform(XMFLOAT4X4 transform);
	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);

private:
	//Transform Properties
	XMFLOAT4X4 mTransform;
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;

};

