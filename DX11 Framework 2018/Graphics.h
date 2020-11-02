#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "DDSTextureLoader.h"

#include <string>
#include <vector>

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 AmbientLight;
	XMFLOAT3 LightVec3;
	float padding;
	XMFLOAT4 SpecularMtrl;
	XMFLOAT4 SpecularLight;
	float SpecularPower;
	XMFLOAT3 padding2;
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

	void SetPyramidBuffer();
	void SetCubeBuffer();
	void SetPlaneBuffer();

	void EnableWireframe(bool enabled);
	
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void UpdateBuffers(XMFLOAT4X4& position, float t);

	void SetShaders();

private:
	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

	XMFLOAT3 lightDirection;

	XMFLOAT4 diffuseMaterial;
	XMFLOAT4 diffuseLight;

	XMFLOAT4 ambientMaterial;
	XMFLOAT4 ambientLight;

	XMFLOAT4 specularMtrl;
	XMFLOAT4 specularLight;
	XMVECTOR eyePos;
	float specularPower;

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

	ID3D11Buffer* _pPyramidVertexBuffer;
	ID3D11Buffer* _pPyramidIndexBuffer;
	
	ID3D11Buffer* _pPlaneIndexBuffer;
	ID3D11Buffer* _pPlaneVertexBuffer;

	ID3D11Buffer* _pCubeVertexBuffer;
	ID3D11Buffer* _pCubeIndexBuffer;

	ID3D11ShaderResourceView* _pDiffuseTexture = nullptr;
	ID3D11ShaderResourceView* _pSpecularTexture = nullptr;
	ID3D11SamplerState* _pSamplerLinear = nullptr;

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

	//Vertex Buffer Methods
	HRESULT InitVertexBuffer();
	void InitCubeVertexBuffer();
	void InitPyramidVertexBuffer();
	
	//Generate Plane methods
	ID3D11Buffer* GeneratePlaneVertices(float width, float depth, int rows, int columns);
	ID3D11Buffer* GeneratePlaneIndices(int height, int width);
	void GeneratePlane(float width, float depth, int rows, int columns);

	//Index Buffer Methods
	HRESULT InitIndexBuffer();
	void InitCubeIndexBuffer();
	void InitPyramidIndexBuffer();
	
	//Switching shapes 
	void SwitchVertexBuffer(ID3D11Buffer* buffer);
	void SwitchIndexBuffer(ID3D11Buffer* buffer);


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

