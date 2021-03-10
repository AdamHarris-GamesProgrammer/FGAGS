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
#include "ImGUIManager.h"
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

	//Initialize Functions
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);
	LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//Drawing Related Functions
	void ClearBuffers();
	void Draw(unsigned int indexCount);
	void Present();
	void BeginFrame();
	void EndFrame();

	//Light Control Window
	void LightingWindow();

	//Mouse Control Settings
	void HideCursor();
	void ShowCursor();
	void ConfineCursor();
	void FreeCursor();


	//Setters
	void SetConstantBuffer();
	void SetObjectBuffers(Material mat, XMFLOAT4X4& position);

	//Rasterizer State Setters
	void SetWireframe(bool enabled);
	void SetFrontFaceCulling();
	void SetCurrentRSState();

	//Blender State Setters
	void SetSolidBlend();
	void SetTransparentBlend();

	//Render Clear Colour Setter
	void SetClearColor(float* color);

	//Current Camera Setter
	void SetCurrentCamera(std::shared_ptr<Camera> newCamera);


	//Getters
	UINT GetWindowWidth() const;
	UINT GetWindowHeight() const;

	int GetMouseX() const;
	int GetMouseY() const;

	int GetMouseClickX() const;
	int GetMouseClickY() const;

	bool IsLeftMouseDown() const;

	HWND& GetWnd();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;



private:
	//Mouse Event
	void OnMouseMove(WPARAM btnState, int x, int y);

	//Initialization Functions
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	HRESULT InitSampler();
	HRESULT InitBlendState();
	HRESULT InitRenderTarget();
	HRESULT InitDepthBuffer();
	HRESULT InitSwapChain();
	HRESULT InitWireframeView();
	HRESULT InitSolidView();
	HRESULT InitFrontCulling();
	HRESULT InitConstantBuffer();

	void InitViewport();

	//Memory Cleanup
	void Cleanup();

	//Short hand for checking HRESULT error codes
	bool CheckResult(int in);

private:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pDeviceContext = nullptr;


	HINSTANCE               mInstance = nullptr;
	HWND                    mWindow = nullptr;

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

	//Window Size
	UINT mWindowHeight;
	UINT mWindowWidth;

	//Mouse Position
	int mMouseX = 0;
	int mMouseY = 0;

	int _mouseClickX = 0;
	int _mouseClickY = 0;

	bool _isLeftMouseDown = false;

	//Render Clear Colour
	float mClearColor[4] = { 0.5f,0.5f,0.5f,1.0f };

	//Scene Lighting Variables
	DirectionalLight mDirectionalLight;
	PointLight mPointLight;
	SpotLight mSpotLight;

	ImGUIManager mImGuiManager;

};

