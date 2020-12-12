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

	void SwitchCamera(Camera* newCamera);

	void EnableWireframe(bool enabled);
	
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void UpdateBuffers(Material mat, XMFLOAT4X4& position);

	void SetConstantBuffer();

	void SetFrontFaceCulling();
	void SetBackFaceCulling();

	void SetSolidBlend();
	void SetTransparentBlend();
	
	UINT GetWindowWidth();
	UINT GetWindowHeight();



	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void SetClearColor(float* color);

	int GetMouseX();
	int GetMouseY();

	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND& GetWnd();

private:
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseDown(WPARAM btnState, int x, int y);

	int mMouseX = 0;
	int mMouseY = 0;

private:

	float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };

	DirectionalLight mDirectionalLight;
	PointLight mPointLight;
	SpotLight mSpotLight;
	
	HINSTANCE               _hInst;
	HWND                    _hWnd;

	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device* _pd3dDevice;

	ID3D11DeviceContext* _pImmediateContext;
	IDXGISwapChain* _pSwapChain;

	ID3D11RenderTargetView* _pRenderTargetView;

	ID3D11SamplerState* _pSamplerLinear = nullptr;

	ID3D11Buffer* _pConstantBuffer;

	ID3D11RasterizerState* _wireFrame;
	ID3D11RasterizerState* _solid;
	ID3D11RasterizerState* mFrontFaceCulling;

	ID3D11BlendState* mBlendState;

	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D* _depthStencilBuffer;

	Camera* mCurrentCamera;

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

	UINT _WindowHeight;
	UINT _WindowWidth;

};

