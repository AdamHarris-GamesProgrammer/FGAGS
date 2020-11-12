#include "Camera.h"


Camera::Camera()
{

}

Camera::Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	mEye = position;
	mAt = at;
	mUp = up;

	mWindowHeight = windowHeight;
	mWindowWidth = windowWidth;
	mNearDepth = nearDepth;
	mFarDepth = farDepth;

	Reshape(windowWidth, windowHeight, nearDepth, farDepth);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	XMVECTOR  eye = XMVectorSet(mEye.x, mEye.y, mEye.z, 0.0f);
	XMVECTOR  at = XMVectorSet(mAt.x, mAt.y, mAt.z, 0.0f);
	XMVECTOR  up = XMVectorSet(mUp.x, mUp.y, mUp.z, 0.0f);


	if (mLookTo) {
		XMStoreFloat4x4(&mView, XMMatrixLookToLH(eye, at, up));
	}
	else
	{
		XMStoreFloat4x4(&mView, XMMatrixLookAtLH(eye, at, up));
	}

	
}

DirectX::XMFLOAT3 Camera::GetEye()
{
	return mEye;
}

DirectX::XMFLOAT3 Camera::GetUp()
{
	return mUp;
}

DirectX::XMFLOAT3 Camera::GetAt()
{
	return mAt;
}

DirectX::XMFLOAT4X4 Camera::GetProjection()
{
	return mProjection;
}

DirectX::XMFLOAT4X4 Camera::GetView()
{
	return mView;
}

void Camera::SetLookTo(bool val)
{
	mLookTo = val;
}

void Camera::SetEye(XMFLOAT3 inPos)
{
	mEye = inPos;
}

void Camera::SetUp(XMFLOAT3 inPos)
{
	mUp = inPos;
}

void Camera::SetAt(XMFLOAT3 inPos)
{
	mAt = inPos;
}

void Camera::Reshape(UINT windowWidth, UINT windowHeight, FLOAT nearDepth, FLOAT farDepth)
{
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;
	mNearDepth = nearDepth;
	mFarDepth = farDepth;

	XMStoreFloat4x4(&mProjection, XMMatrixPerspectiveFovLH(XM_PIDIV2, mWindowWidth / (FLOAT)mWindowHeight, 0.001f, 100.0f));
}
