#pragma once
#include "Camera.h"
#include "GameObject.h"

class FollowCamera : public Camera
{
public:
	FollowCamera(GameObject* objectToFollow, XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight,
		FLOAT nearDepth, FLOAT farDepth);

	void Update() override;
	void PollInput(float dt) override;

	void SetOffset(XMFLOAT3 offset);

	void SetNewFollowTarget(GameObject* newTarget);
private:
	GameObject* mFollowTarget = nullptr;

	XMFLOAT3 mOffset;
};

