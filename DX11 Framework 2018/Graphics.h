#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "DDSTextureLoader.h"
#include <windowsx.h>

#include "LightingStructs.h"
#include "Camera.h"
#include "VertexStruct.h"
#include "OBJLoader.h"

#include <string>
#include <vector>

using namespace DirectX;

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	Material ObjectMaterial;
	DirectionalLight DirectionalLight;
	PointLight PointLight;
	SpotLight SpotLight;
	XMFLOAT3 EyePosW;
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	void ClearBuffers();
	void Draw(unsigned int indexCount);
	void Present();


	void HideCursor();
	void ShowCursor();
	void ConfineCursor();
	void FreeCursor();

	void LightColourOptions(Light* light);
	void LightingWindow();

	void SwitchCamera(std::shared_ptr<Camera> newCamera);

	void EnableWireframe(bool enabled);

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void UpdateBuffers(Material mat, XMFLOAT4X4& position);


	void SetConstantBuffer();

	void SetFrontFaceCulling();
	void SetCurrentRSState();

	void SetSolidBlend();
	void SetTransparentBlend();

	void SetClearColor(float* color);

	UINT GetWindowWidth() const;
	UINT GetWindowHeight() const;

	int GetMouseX() const;
	int GetMouseY() const;

	HWND& GetWnd();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;





	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);


	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();

	void InitializeSampler();

	HRESULT InitRenderTarget();
	void Cleanup();

	HRESULT InitDepthBuffer();
	HRESULT InitSwapChain();
	HRESULT InitWireframeView();
	HRESULT InitSolidView();
	HRESULT InitFrontCulling();
	HRESULT InitConstantBuffer();
	void InitViewport();

	bool CheckResult(int in);

private:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;


	HINSTANCE               mInstance;
	HWND                    mWindow;

	D3D_DRIVER_TYPE         mDriverType;
	D3D_FEATURE_LEVEL       mfeatureLevel;

	IDXGISwapChain* pSwapChain = nullptr;

	ID3D11RenderTargetView* pRenderTargetView = nullptr;

	ID3D11SamplerState* pLinearSampler = nullptr;

	ID3D11Buffer* pConstantBuffer = nullptr;

	ID3D11RasterizerState* pWireframeRSState = nullptr;
	ID3D11RasterizerState* pSolidRSState = nullptr;
	ID3D11RasterizerState* pFrontFaceRSState = nullptr;
	ID3D11RasterizerState* pCurrentRSState = nullptr;

	ID3D11BlendState* pBlendState = nullptr;

	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	ID3D11Texture2D* pDepthStencilBuffer = nullptr;

	std::shared_ptr<Camera> pCurrentCamera;


	UINT mWindowHeight;
	UINT mWindowWidth;

	int mMouseX = 0;
	int mMouseY = 0;

	float mClearColor[4] = { 0.5f,0.5f,0.5f,1.0f };

	DirectionalLight mDirectionalLight;
	PointLight mPointLight;
	SpotLight mSpotLight;

};

