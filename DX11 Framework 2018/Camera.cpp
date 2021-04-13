#include "Camera.h"

//Initializes all variables to sensible values and sets the lens
Camera::Camera()
	: 
	_right(1.0f,0.0f,0.0f),
	_up(0.0,1.0f,0.0f),
	_look(0.0f,0.0f,1.0f)
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

	Vector3 tPos = _pTransform->GetPosition();
	XMFLOAT3 fPos = tPos;
	//Stores the position, look, right and up vectors
	XMStoreFloat3(&fPos, pos);
	_pTransform->SetPosition(fPos);
	XMStoreFloat3(&_look, l);
	XMStoreFloat3(&_right, r);
	XMStoreFloat3(&_up, u);
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
	XMVECTOR r = XMLoadFloat3(&_right);

	Vector3 vPos = _pTransform->GetPosition();
	XMFLOAT3 fPos = vPos;

	XMVECTOR p = XMLoadFloat3(&fPos);
	XMStoreFloat3(&fPos, XMVectorMultiplyAdd(s, r, p));
	_pTransform->SetPosition(fPos);
}

//Walks the camera forwards and backwards based on the look vector
void Camera::Walk(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR i = XMLoadFloat3(&_look);

	Vector3 vPos = _pTransform->GetPosition();
	XMFLOAT3 fPos = vPos;

	XMVECTOR p = XMLoadFloat3(&fPos);
	XMStoreFloat3(&fPos, XMVectorMultiplyAdd(s, i, p));
	_pTransform->SetPosition(fPos);
}

//Pitches the camera up and down
void Camera::Pitch(float angle)
{
	XMMATRIX r = XMMatrixRotationAxis(XMLoadFloat3(&_right), angle);

	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), r));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), r));
}

//Rotates the camera on the Y axis allowing it to turn
void Camera::RotateY(float angle)
{
	XMMATRIX r = XMMatrixRotationY(angle);

	XMStoreFloat3(&_right, XMVector3TransformNormal(XMLoadFloat3(&_right), r));
	XMStoreFloat3(&_up, XMVector3TransformNormal(XMLoadFloat3(&_up), r));
	XMStoreFloat3(&_look, XMVector3TransformNormal(XMLoadFloat3(&_look), r));
}

//Updates the cameras view matrix 
void Camera::Update(float dt)
{
	//Loads all the needed vectors
	XMVECTOR r = XMLoadFloat3(&_right);
	XMVECTOR u = XMLoadFloat3(&_up);
	XMVECTOR l = XMLoadFloat3(&_look);

	Vector3 vPos = _pTransform->GetPosition();
	XMFLOAT3 fPos = vPos;

	XMVECTOR p = XMLoadFloat3(&fPos);

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
	XMStoreFloat3(&_right, r);
	XMStoreFloat3(&_up, u);
	XMStoreFloat3(&_look, l);

	//Sets the view matrix properties
	_view(0, 0) = _right.x;
	_view(1, 0) = _right.y;
	_view(2, 0) = _right.z;
	_view(3, 0) = x;

	_view(0, 1) = _up.x;
	_view(1, 1) = _up.y;
	_view(2, 1) = _up.z;
	_view(3, 1) = y;

	_view(0, 2) = _look.x;
	_view(1, 2) = _look.y;
	_view(2, 2) = _look.z;
	_view(3, 2) = z;

	_view(0, 3) = 0.0f;
	_view(1, 3) = 0.0f;
	_view(2, 3) = 0.0f;
	_view(3, 3) = 1.0f;

}

#pragma region Getters
DirectX::XMVECTOR Camera::GetRightXM() const
{
	return XMLoadFloat3(&_right);
}

DirectX::XMFLOAT3 Camera::GetRight() const
{
	return _right;
}

DirectX::XMVECTOR Camera::GetUpXM() const
{
	return XMLoadFloat3(&_up);
}

DirectX::XMFLOAT3 Camera::GetUp() const
{
	return _up;
}

DirectX::XMVECTOR Camera::GetLookXM() const
{
	return XMLoadFloat3(&_look);
}

DirectX::XMFLOAT3 Camera::GetLook() const
{
	return _look;
}

float Camera::GetNearZ() const
{
	return _nearZ;
}

float Camera::GetFarZ() const
{
	return _farZ;
}

float Camera::GetAspect() const
{
	return _aspect;
}

float Camera::GetFovY() const
{
	return _fovY;
}

float Camera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / _nearZ);
}

float Camera::GetNearWindowHeight() const
{
	return _nearWindowHeight;
}

float Camera::GetNearWindowWidth() const
{
	return _aspect * _nearWindowHeight;
}

float Camera::GetFarWindowHeight() const
{
	return _farWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return _aspect * _farWindowHeight;
}

DirectX::XMMATRIX Camera::View() const
{
	return XMLoadFloat4x4(&_view);
}

DirectX::XMMATRIX Camera::Proj() const
{
	return XMLoadFloat4x4(&_proj);
}

DirectX::XMMATRIX Camera::ViewProj() const
{
	return XMMatrixMultiply(View(), Proj());
}

#pragma endregion

#pragma region Setters
void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	_fovY = fovY;
	_aspect = aspect;

	_nearZ = zn;
	_farZ = zf;

	_nearWindowHeight = 2.0f * _nearZ * tanf(0.5f * _fovY);
	_farWindowHeight = 2.0f * _farZ * tanf(0.5f * _fovY);

	XMMATRIX projection = XMMatrixPerspectiveFovLH(_fovY, _aspect, _nearZ, _farZ);

	XMStoreFloat4x4(&_proj, projection);
}
#pragma endregion