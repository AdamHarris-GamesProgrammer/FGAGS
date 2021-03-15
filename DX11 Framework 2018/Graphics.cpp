#include "Graphics.h"
#include <windowsx.h>
#include "ImGui/imgui.h"
#include "Imgui/imgui_impl_win32.h"

namespace
{
	// This is just used to forward Windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.
	Graphics* gfx = 0;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	return gfx->WndProc(hwnd, msg, wParam, lParam);
}

Graphics::Graphics()
{
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;

	gfx = this;

	
}

Graphics::~Graphics() {
	Cleanup();
}

HRESULT Graphics::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	//returns a failure error if something goes wrong
	if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
		return E_FAIL;
	}

	//defines the window size
	RECT rc;
	GetClientRect(_window, &rc);
	_windowWidth = rc.right - rc.left;
	_windowHeight = rc.bottom - rc.top;

	//if failed to init the window return a fail
	if (FAILED(InitDevice()))
	{
		Cleanup();

		return E_FAIL;
	}

	//Sets Directional light properties
	_directionalLight.Direction = XMFLOAT3(0.577f, -0.577f, 0.577f);
	_directionalLight.Diffuse = XMFLOAT4(0.99f, 0.92f, 0.8f, 1.0f);
	_directionalLight.Ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	_directionalLight.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 0.7f);

	//Sets point light properties
	_pointLight.Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	_pointLight.Diffuse = XMFLOAT4(0.7f, 0.0f, 0.0f, 1.0f);
	_pointLight.Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 0.6f);
	_pointLight.Position = XMFLOAT3(0.0f, 3.0f, 0.0f);
	_pointLight.Attenuation = XMFLOAT3(0.0f, 0.1f, 0.0f);
	_pointLight.Range = 8.0f;

	//Sets spot light properties
	_spotLight.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	_spotLight.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	_spotLight.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_spotLight.Attenuation = XMFLOAT3(1.0f, 0.0f, 0.0f);
	_spotLight.Spot = 96.0f;
	_spotLight.Range = 10000.0f;
	_spotLight.Direction = XMFLOAT3(1.0f, 0.1f, 0.0f);
	_spotLight.Position = XMFLOAT3(0.0f, 1.0f, 0.0f);

	_ImGuiManager = ImGUIManager(_pDevice, _pDeviceContext, _window);

	return S_OK;
}

//ImGUI windows event handler, enables us to use the mouse in imgui
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Graphics::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_QUIT:

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONDOWN:
		_isLeftMouseDown = true;
		const POINTS pt = MAKEPOINTS(lParam);
		_mouseClickX = pt.x;
		_mouseClickY = pt.y;
		break;
	case  WM_LBUTTONUP:
		_isLeftMouseDown = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Graphics::LightingWindow()
{
	ImGui::Begin("Lighting Control Panel");

	ImGui::Text("Directional Light");

	if (ImGui::TreeNode("Directional Light")) {
		_directionalLight.Controls();
	}

	if (ImGui::TreeNode("Point Light")) {
		_pointLight.Controls();
	}

	if (ImGui::TreeNode("Spot Light")) {
		_spotLight.Controls();
	}

	ImGui::End();
}


void Graphics::Cleanup()
{
	//Releases all memory associated with DirectX Member variables
	if (_pDeviceContext) _pDeviceContext->ClearState();
	if (_pDeviceContext) _pDeviceContext->Release();
	if (_pDevice) _pDevice->Release();

	if (_pSwapChain) _pSwapChain->Release();

	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pLinearSampler) _pLinearSampler->Release();

	if (_pConstantBuffer) _pConstantBuffer->Release();

	if (_pWireframeRSState) _pWireframeRSState->Release();
	if (_pSolidRSState) _pSolidRSState->Release();
	if (_pFrontFaceRSState) _pFrontFaceRSState->Release();

	if (_pBlendState) _pBlendState->Release();

	if (_pDepthStencilView) _pDepthStencilView->Release();
	if (_pDepthStencilBuffer) _pDepthStencilBuffer->Release();
}

#pragma region InitializationFunctions
HRESULT Graphics::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	_instance = hInstance;
	//sets window size
	RECT rc = { 0, 0, 1280, 720 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_window = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!_window)
		return E_FAIL;

	//display the window on the screen
	ShowWindow(_window, nCmdShow);


	return S_OK;
}

HRESULT Graphics::InitDevice()
{
	if (!(CheckResult(InitSwapChain()) && CheckResult(InitDepthBuffer()) && CheckResult(InitRenderTarget()))) {
		MessageBox(_window, L"Initialization of device failed", L"Error", MB_ICONERROR);
		return S_FALSE;
	}

	if (!(CheckResult(InitWireframeView()) && CheckResult(InitSolidView()) && CheckResult(InitFrontCulling()))) {
		MessageBox(_window, L"Initialization of rasterizer states failed", L"Error", MB_ICONERROR);
		return S_FALSE;
	}

	if (!(CheckResult(InitConstantBuffer()) && CheckResult(InitSampler()))) {
		MessageBox(_window, L"Initialization of Constant Buffer or Sampler Failed", L"Error", MB_ICONERROR);
		return S_FALSE;
	}

	if (!CheckResult(InitBlendState())) {
		MessageBox(_window, L"Initialization of Blend State Failed", L"Error", MB_ICONERROR);
		return S_FALSE;
	}

	InitViewport();

	_pCurrentRSState = _pSolidRSState;

	return S_OK;
}

HRESULT Graphics::InitSampler()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = _pDevice->CreateSamplerState(&sampDesc, &_pLinearSampler);

	return hr;
}

HRESULT Graphics::InitBlendState()
{
	HRESULT hr;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.RenderTarget[0] = rtbd;

	hr = _pDevice->CreateBlendState(&blendDesc, &_pBlendState);

	return hr;
}

HRESULT Graphics::InitRenderTarget()
{
	HRESULT hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		return hr;

	hr = _pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	pBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	_pDeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);

	// Set primitive topology
	_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//C4
HRESULT Graphics::InitDepthBuffer()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = _windowWidth;
	depthStencilDesc.Height = _windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_pDepthStencilBuffer);
	return _pDevice->CreateDepthStencilView(_pDepthStencilBuffer, nullptr, &_pDepthStencilView);
}

HRESULT Graphics::InitSwapChain()
{
	HRESULT hr;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _windowWidth;
	sd.BufferDesc.Height = _windowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _window;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pDevice, &_featureLevel, &_pDeviceContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;
}

HRESULT Graphics::InitWireframeView()
{
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	return _pDevice->CreateRasterizerState(&wfdesc, &_pWireframeRSState);
}

HRESULT Graphics::InitSolidView()
{
	D3D11_RASTERIZER_DESC sodesc;
	ZeroMemory(&sodesc, sizeof(D3D11_RASTERIZER_DESC));
	sodesc.FillMode = D3D11_FILL_SOLID;
	sodesc.CullMode = D3D11_CULL_BACK;
	return _pDevice->CreateRasterizerState(&sodesc, &_pSolidRSState);
}

HRESULT Graphics::InitFrontCulling()
{
	D3D11_RASTERIZER_DESC sodesc;
	ZeroMemory(&sodesc, sizeof(D3D11_RASTERIZER_DESC));
	sodesc.FillMode = D3D11_FILL_SOLID;
	sodesc.CullMode = D3D11_CULL_FRONT;
	return _pDevice->CreateRasterizerState(&sodesc, &_pFrontFaceRSState);
}

HRESULT Graphics::InitConstantBuffer()
{
	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	return _pDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);
}

void Graphics::InitViewport()
{
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_windowWidth;
	vp.Height = (FLOAT)_windowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pDeviceContext->RSSetViewports(1, &vp);
}

bool Graphics::CheckResult(int in)
{
	if (in < 0) { return false; }
	return true;
}
#pragma endregion

#pragma region MouseFunctions
void Graphics::OnMouseMove(WPARAM btnState, int x, int y)
{
	_mouseX = x;
	_mouseY = y;
}

void Graphics::HideCursor()
{
	::ShowCursor(false);
}

void Graphics::ShowCursor()
{
	::ShowCursor(true);
}

void Graphics::ConfineCursor()
{
	RECT rect;
	GetClientRect(_window, &rect);
	MapWindowPoints(_window, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Graphics::FreeCursor()
{
	ClipCursor(nullptr);
}
#pragma endregion

#pragma region Drawing Related Functions
void Graphics::ClearBuffers()
{
	_pDeviceContext->ClearRenderTargetView(_pRenderTargetView, _clearColor);

	//C4
	_pDeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphics::Draw(unsigned int indexCount)
{
	//Draws object to swap chain buffer
	_pDeviceContext->DrawIndexed(indexCount, 0, 0);
}

void Graphics::Present()
{
	// Present our back buffer to our front buffer
	_pSwapChain->Present(0, 0);
}

void Graphics::BeginFrame()
{
	_ImGuiManager.BeginFrame();
	ClearBuffers();
	SetConstantBuffer();
	SetSolidBlend();
}

void Graphics::EndFrame()
{
	_ImGuiManager.EndFrame();
	Present();
}

#pragma endregion

#pragma region Setters
void Graphics::SetCurrentCamera(std::shared_ptr<Camera> newCamera)
{
	_pCurrentCamera = newCamera;
}

void Graphics::SetWireframe(bool enabled)
{
	ID3D11RasterizerState* renderState;
	enabled ? renderState = _pWireframeRSState : renderState = _pSolidRSState;
	_pDeviceContext->RSSetState(renderState);
	_pCurrentRSState = renderState;
}

void Graphics::SetConstantBuffer()
{
	_pDeviceContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pDeviceContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pDeviceContext->PSSetSamplers(0, 1, &_pLinearSampler);
}

void Graphics::SetFrontFaceCulling()
{
	_pDeviceContext->RSSetState(_pFrontFaceRSState);
}

void Graphics::SetCurrentRSState()
{
	_pDeviceContext->RSSetState(_pCurrentRSState);
}

void Graphics::SetSolidBlend()
{
	_pDeviceContext->OMSetBlendState(0, 0, 0xFFFFFFFF);
}

void Graphics::SetTransparentBlend()
{
	float blendFactor[] = { 0.75f,0.75f,0.75f,1.0f };

	_pDeviceContext->OMSetBlendState(_pBlendState, blendFactor, 0xFFFFFFFF);
}

void Graphics::SetObjectBuffers(Material mat, XMFLOAT4X4& position)
{
	ConstantBuffer cb;

	//Sets the View, Projection, and World Matrices
	cb.mView = XMMatrixTranspose(_pCurrentCamera->View());
	cb.mProjection = XMMatrixTranspose(_pCurrentCamera->Proj());
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&position));

	//Material Constant Buffers
	cb.ObjectMaterial = mat;

	//Light Constant Buffers
	cb.DirectionalLight = _directionalLight;
	cb.PointLight = _pointLight;
	cb.SpotLight = _spotLight;

	//Camera Position
	cb.EyePosW = (XMFLOAT3)_pCurrentCamera->GetTransform().GetPosition();

	//Updates the constant buffer with the new values
	_pDeviceContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
}

void Graphics::SetClearColor(float* color)
{
	_clearColor[0] = color[0];
	_clearColor[1] = color[1];
	_clearColor[2] = color[2];
	_clearColor[3] = color[3];
}
#pragma endregion

#pragma region Getters
UINT Graphics::GetWindowWidth() const
{
	return _windowWidth;
}

UINT Graphics::GetWindowHeight() const
{
	return _windowHeight;
}

int Graphics::GetMouseX() const
{
	return _mouseX;
}

int Graphics::GetMouseY() const
{
	return _mouseY;
}

int Graphics::GetMouseClickX() const
{
	return _mouseClickX;
}

int Graphics::GetMouseClickY() const
{
	return _mouseClickY;
}

bool Graphics::IsLeftMouseDown() const
{
	return _isLeftMouseDown;
}

ID3D11Device* Graphics::GetDevice() const
{
	return _pDevice;
}

ID3D11DeviceContext* Graphics::GetDeviceContext() const
{
	return _pDeviceContext;
}

HWND& Graphics::GetWnd()
{
	return _window;
}
#pragma endregion