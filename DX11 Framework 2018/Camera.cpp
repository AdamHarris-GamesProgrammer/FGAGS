#include "Camera.h"

//Initializes all variables to sensible values and sets the lens
Camera::Camera()
	: mPosition(0.0f,0.0f,0.0f),
	mRight(1.0f,0.0f,0.0f),
	mUp(0.0,1.0f,0.0f),
	mLook(0.0f,0.0f,1.0f)
{
	SetLens(0.25f * 3.1452, 1.0f, 1.0f, 1000.0f);
}

/// <summary>
/// Looks at a certain point in space
/// </summary>
/// <param name="pos">Position of the camera</param>
/// <param name="target">Position of the target</param>
/// <param name="worldUp">Up vector</param>
void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	//Calculates the look, right and up vectors
	XMVECTOR l = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR r = XMVector3Normalize(XMVector3Cross(worldUp, l));
	XMVECTOR u = XMVector3Cross(l, r);

	//Stores the position, look, right and up vectors
	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, l);
	XMStoreFloat3(&mRight, r);
	XMStoreFloat3(&mUp, u);
}

//Looks at a point in space and moves the camera to a designated area
void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR p = XMLoadFloat3(&pos);
	XMVECTOR t = XMLoadFloat3(&target);
	XMVECTOR u = XMLoadFloat3(&up);

	LookAt(p, t, u);
}

//Strafes the camera left or right
void Camera::Strafe(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

//Walks the camera forwards and backwards based on the look vector
void Camera::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR i = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, i, p));
}

//Pitches the camera up and down
void Camera::Pitch(float angle)
{
	XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), r));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), r));
}

//Rotates the camera on the Y axis allowing it to turn
void Camera::RotateY(float angle)
{
	XMMATRIX r = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), r));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), r));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), r));
}

//Updates the cameras view matrix 
void Camera::UpdateViewMatrix()
{
	//Loads all the needed vectors
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR u = XMLoadFloat3(&mUp);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	//normalizes the look vector
	l = XMVector3Normalize(l);

	//normalizes the cross product of the look and right vectors
	u = XMVector3Normalize(XMVector3Cross(l, r));

	//Gets the cross product of the up and look vectors
	r = XMVector3Cross(u, l);

	//Gets the x,y,z values of the camera
	float x = -XMVectorGetX(XMVector3Dot(p, r));
	float y = -XMVectorGetX(XMVector3Dot(p, u));
	float z = -XMVectorGetX(XMVector3Dot(p, l));

	//Stores the right, up, and look vectors
	XMStoreFloat3(&mRight, r);
	XMStoreFloat3(&mUp, u);
	XMStoreFloat3(&mLook, l);

	//Sets the view matrix properties
	mView(0, 0) = mRight.x;
	mView(1, 0) = mRight.y;
	mView(2, 0) = mRight.z;
	mView(3, 0) = x;

	mView(0, 1) = mUp.x;
	mView(1, 1) = mUp.y;
	mView(2, 1) = mUp.z;
	mView(3, 1) = y;

	mView(0, 2) = mLook.x;
	mView(1, 2) = mLook.y;
	mView(2, 2) = mLook.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;

}

#pragma region Getters
DirectX::XMVECTOR Camera::GetRightXM() const
{
	return XMLoadFloat3(&mRight);
}

DirectX::XMFLOAT3 Camera::GetRight() const
{
	return mRight;
}

DirectX::XMVECTOR Camera::GetUpXM() const
{
	return XMLoadFloat3(&mUp);
}

DirectX::XMFLOAT3 Camera::GetUp() const
{
	return mUp;
}

DirectX::XMVECTOR Camera::GetLookXM() const
{
	return XMLoadFloat3(&mLook);
}

DirectX::XMFLOAT3 Camera::GetLook() const
{
	return mLook;
}

float Camera::GetNearZ() const
{
	return mNearZ;
}

float Camera::GetFarZ() const
{
	return mFarZ;
}

float Camera::GetAspect() const
{
	return mAspect;
}

float Camera::GetFovY() const
{
	return mFovY;
}

float Camera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float Camera::GetNearWindowHeight() const
{
	return mNearWindowHeight;
}

float Camera::GetNearWindowWidth() const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetFarWindowHeight() const
{
	return mFarWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return mAspect * mFarWindowHeight;
}

DirectX::XMMATRIX Camera::View() const
{
	return XMLoadFloat4x4(&mView);
}

DirectX::XMMATRIX Camera::Proj() const
{
	return XMLoadFloat4x4(&mProj);
}

DirectX::XMMATRIX Camera::ViewProj() const
{
	return XMMatrixMultiply(View(), Proj());
}

DirectX::XMVECTOR Camera::GetPositionXM() const
{
	return XMLoadFloat3(&mPosition);
}

DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return mPosition;
}
#pragma endregion

#pragma region Setters
void Camera::SetPosition(float x, float y, float z)
{
	mPosition = { x,y,z };
}

void Camera::SetPosition(const XMFLOAT3& val)
{
	mPosition = val;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	mFovY = fovY;
	mAspect = aspect;

	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMMATRIX projection = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);

	XMStoreFloat4x4(&mProj, projection);
}
#pragma endregion