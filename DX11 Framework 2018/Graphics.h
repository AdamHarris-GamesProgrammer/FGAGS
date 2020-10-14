#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <wrl.h>
#include "Structs.h"
#include "VertexBuffer.h"

#include <string>

using namespace DirectX;



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

	ID3D11Device* GetDevice() {
		return _pd3dDevice.Get();
	}

	ID3D11DeviceContext* GetContext() {
		return _pImmediateContext.Get();
	}

	void SetShaders();
private:
	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };


	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;

	Microsoft::WRL::ComPtr<ID3D11Device> _pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _pImmediateContext;
	IDXGISwapChain* _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11InputLayout* _pVertexLayout;

	ID3D11Buffer* _pPyramidVertexBuffer;
	ID3D11Buffer* _pPyramidIndexBuffer;

	ID3D11Buffer* _pCubeVertexBuffer;
	ID3D11Buffer* _pCubeIndexBuffer;
	ID3D11Buffer* _pConstantBuffer;

	VertexBuffer* pVertexTest;

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
