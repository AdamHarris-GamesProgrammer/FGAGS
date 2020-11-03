#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(XMFLOAT4X4 inPos);

	XMFLOAT4X4 GetMatrix();
	void SetMatrix(XMFLOAT4X4 inPos);

	XMVECTOR GetEye();
	XMVECTOR GetUp();
	XMVECTOR GetAt();

	void SetEye(XMVECTOR inPos);
	void SetUp(XMVECTOR inPos);
	void SetAt(XMVECTOR inPos);

private:
	XMFLOAT4X4 mPosition;
	XMVECTOR mEye;
	XMVECTOR mAt;
	XMVECTOR mUp;

};

