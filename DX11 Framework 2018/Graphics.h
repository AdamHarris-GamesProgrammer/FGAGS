#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"

#include <string>

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

class Graphics
{
public:
	Graphics();
	~Graphics();

	void ClearBuffers();
	void Draw(unsigned int indexCount, XMFLOAT4X4* position);
	void Present();

	void EnableWireframe(bool enabled);
	
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);


	void SetShaders();
private:
	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };


	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device* _pd3dDevice;
	ID3D11DeviceContext* _pImmediateContext;
	IDXGISwapChain* _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11InputLayout* _pVertexLayout;
	ID3D11Buffer* _pVertexBuffer;
	ID3D11Buffer* _pIndexBuffer;
	ID3D11Buffer* _pConstantBuffer;

	ID3D11RasterizerState* _wireFrame;
	ID3D11RasterizerState* _solid;

	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D* _depthStencilBuffer;

	XMFLOAT4X4              _view;
	XMFLOAT4X4              _projection;

	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();

	HRESULT InitRenderTarget();


	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();
	HRESULT InitDepthBuffer();
	HRESULT InitSwapChain();
	HRESULT InitWireframeView();
	HRESULT InitSolidView();
	HRESULT InitConstantBuffer();
	void InitViewport();


	bool CheckResult(int in);

	UINT _WindowHeight;
	UINT _WindowWidth;

};

