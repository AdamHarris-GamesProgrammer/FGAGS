#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight,
		FLOAT nearDepth, FLOAT farDepth);
	~Camera();

	virtual void Update();
	virtual void PollInput(float dt);

	XMFLOAT3 GetEye();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetAt();
	XMFLOAT4X4 GetProjection();
	XMFLOAT4X4 GetView();

	//void SetLookTo(bool val);
	void SetEye(XMFLOAT3 inPos);
	void SetUp(XMFLOAT3 inPos);
	void SetAt(XMFLOAT3 inPos);

	void Reshape(UINT windowWidth, UINT windowHeight, FLOAT nearDepth, FLOAT farDepth);

protected:
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProjection;

	UINT mWindowWidth;
	UINT mWindowHeight;
	FLOAT mNearDepth;
	FLOAT mFarDepth;

	XMFLOAT3 mEye;
	XMFLOAT3 mAt;
	XMFLOAT3 mUp;

private:

};

