#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "DDSTextureLoader.h"

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
	
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 AmbientLight;
	XMFLOAT4 SpecularMtrl;
	XMFLOAT4 SpecularLight;
	XMFLOAT3 LightVec3;
	float SpecularPower;
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
	void BindTextures(int startSlot, int count, std::vector<ID3D11ShaderResourceView*> textures);
	void ClearTextures();

	void UpdateCamera();

	void EnableWireframe(bool enabled);
	
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void UpdateBuffers(XMFLOAT4X4& position);

	void SetShaders();
	

	//Switching shapes 
	void SwitchVertexBuffer(ID3D11Buffer* buffer);
	void SwitchIndexBuffer(ID3D11Buffer* buffer);


	//Texture Methods
	HRESULT CreateTexture(wchar_t* filepath, ID3D11ShaderResourceView** texture);

	ID3D11Device* _pd3dDevice;

private:

private:

	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

	XMFLOAT3 lightDirection;

	BasicLight diffuseLight;
	BasicLight ambientLight;
	LightWithIntensity specularLight;
	
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;

	ID3D11DeviceContext* _pImmediateContext;
	IDXGISwapChain* _pSwapChain;

	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11InputLayout* _pVertexLayout;

	ID3D11SamplerState* _pSamplerLinear = nullptr;

	ID3D11Buffer* _pConstantBuffer;

	ID3D11RasterizerState* _wireFrame;
	ID3D11RasterizerState* _solid;

	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D* _depthStencilBuffer;

	Camera mCamera;

	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();

	void InitializeSampler();

	HRESULT InitRenderTarget();


	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();


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

