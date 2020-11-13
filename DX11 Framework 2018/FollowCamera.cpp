#include "FollowCamera.h"

FollowCamera::FollowCamera(GameObject* objectToFollow, XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth)
	: Camera(position, at, up, windowWidth, windowHeight, nearDepth, farDepth)
{
	mFollowTarget = objectToFollow;
}

void FollowCamera::Update()
{
	XMFLOAT3 followPosition = mFollowTarget->GetPosition();

	mEye =
	{
		followPosition.x + mOffset.x,
		followPosition.y + mOffset.y,
		followPosition.z + mOffset.z
	};

	mAt = followPosition;

	XMVECTOR  eye = XMVectorSet(mEye.x, mEye.y, mEye.z, 0.0f);
	XMVECTOR  at = XMVectorSet(mAt.x, mAt.y, mAt.z, 0.0f);
	XMVECTOR  up = XMVectorSet(mUp.x, mUp.y, mUp.z, 0.0f);

	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(eye, at, up));
}

void FollowCamera::PollInput(float dt)
{
	//Zoom In
	if (GetAsyncKeyState(VK_NUMPAD8)) {
		mOffset.z += 1.2f * dt;
	}
	//Zoom Out
	else if (GetAsyncKeyState(VK_NUMPAD5)) {
		mOffset.z -= 1.2f * dt;
	}

	//Pivot Left
	if (GetAsyncKeyState(VK_NUMPAD4)) {
		mOffset.x -= 1.2f * dt;
	}

	//Pivot Right
	else if (GetAsyncKeyState(VK_NUMPAD6)) {
		mOffset.x += 1.2f * dt;
	}
}

void FollowCamera::SetOffset(XMFLOAT3 offset)
{
	mOffset = offset;
}

void FollowCamera::SetNewFollowTarget(GameObject* newTarget)
{
	mFollowTarget = newTarget;
}
