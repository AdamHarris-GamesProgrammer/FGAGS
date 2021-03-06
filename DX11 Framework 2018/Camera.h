#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Object.h"

using namespace DirectX;

class Camera : public Object {
public:
	Camera();

	//Get Camera basis vectors
	XMVECTOR GetRightXM() const;
	XMFLOAT3 GetRight() const;
	XMVECTOR GetUpXM() const;
	XMFLOAT3 GetUp() const;
	XMVECTOR GetLookXM() const;
	XMFLOAT3 GetLook() const;

	//Get frustum properties
	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;
	float GetFovY() const;
	float GetFovX() const;

	//Get near and far plane dimensions in view space coordinates
	float GetNearWindowHeight() const;
	float GetNearWindowWidth() const;
	float GetFarWindowHeight() const;
	float GetFarWindowWidth() const;

	//Set frustum
	void SetLens(float fovY, float aspect, float zn, float zf);

	//Look at targets
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up = XMFLOAT3(0.0f,1.0f,0.0f));

	//Get View/Proj matrices
	XMMATRIX View() const;
	XMMATRIX Proj() const;
	XMMATRIX ViewProj() const;

	//Strafe and Walk
	void Strafe(float d);
	void Walk(float d);

	//Rotate the camera
	void Pitch(float angle);
	void RotateY(float angle);

	//After modifying any camera aspects you need to update the view matrix
	void Update(float dt) override;

private:
	XMFLOAT3 _right;
	XMFLOAT3 _up;
	XMFLOAT3 _look;

	float _nearZ;
	float _farZ;
	float _aspect;
	float _fovY;
	float _nearWindowHeight;
	float _farWindowHeight;

	XMFLOAT4X4 _view;
	XMFLOAT4X4 _proj;
};