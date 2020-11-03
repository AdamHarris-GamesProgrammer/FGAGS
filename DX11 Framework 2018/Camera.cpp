#include "Camera.h"

Camera::Camera(XMFLOAT4X4 inPos)
{
	mPosition = inPos;
}

Camera::Camera()
{

}

DirectX::XMFLOAT4X4 Camera::GetMatrix()
{
	return mPosition;
}

void Camera::SetMatrix(XMFLOAT4X4 inPos)
{
	mPosition = inPos;
}

DirectX::XMVECTOR Camera::GetEye()
{
	return mEye;
}

DirectX::XMVECTOR Camera::GetUp()
{
	return mUp;
}

DirectX::XMVECTOR Camera::GetAt()
{
	return mAt;
}

void Camera::SetEye(XMVECTOR inPos)
{
	mEye = inPos;

	XMStoreFloat4x4(&mPosition, XMMatrixLookAtLH(mEye, mAt, mUp));
}

void Camera::SetUp(XMVECTOR inPos)
{
	mUp = inPos;

	XMStoreFloat4x4(&mPosition, XMMatrixLookAtLH(mEye, mAt, mUp));
}

void Camera::SetAt(XMVECTOR inPos)
{
	mAt = inPos;

	XMStoreFloat4x4(&mPosition, XMMatrixLookAtLH(mEye, mAt, mUp));
}
