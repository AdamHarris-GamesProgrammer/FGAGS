#include "Graphics.h"
#include <windowsx.h>

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
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return gfx->WndProc(hwnd, msg, wParam, lParam);
}

Graphics::Graphics()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pConstantBuffer = nullptr;

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
	GetClientRect(_hWnd, &rc);
	_WindowWidth = rc.right - rc.left;
	_WindowHeight = rc.bottom - rc.top;

	//if failed to init the window return a fail
	if (FAILED(InitDevice()))
	{
		Cleanup();

		return E_FAIL;
	}

	lightDirection = XMFLOAT3(0.0f, 1.0f, -1.0f);
	diffuseLight = BasicLight(XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	ambientLight = BasicLight(XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f), XMFLOAT4(0.2f, 0.2f, 0.2f, 0.2f));
	specularLight = LightWithIntensity(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.3f, 0.3f, 0.5f, 1.0f), 10.0f);

	return S_OK;
}

void Graphics::OnMouseMove(int x, int y)
{
	mMouseX = x;
	mMouseY = y;
}

void Graphics::OnMouseDown(WPARAM btnState, int x, int y)
{
	mMouseX = x;
	mMouseY = y;

	SetCapture(_hWnd);
}

LRESULT Graphics::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

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
		if ((wParam & MK_RBUTTON) != 0) {
			const POINTS pt = MAKEPOINTS(lParam);
			if (pt.x > 0 && pt.x < _WindowWidth && pt.y >= 0 && pt.y < _WindowHeight) {
				OnMouseMove(pt.x, pt.y);

			}
		}


	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

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
	_hInst = hInstance;
	//sets window size
	RECT rc = { 0, 0, 1280, 720 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!_hWnd)
		return E_FAIL;

	//display the window on the screen
	ShowWindow(_hWnd, nCmdShow);


	return S_OK;
}

HRESULT Graphics::InitDevice()
{
	HRESULT hr = S_OK;

	if (!(CheckResult(InitSwapChain())
		&& CheckResult(InitDepthBuffer())
		&& CheckResult(InitRenderTarget()))
		) {
		MessageBox(_hWnd, L"Initialization of device failed", L"Error", MB_ICONERROR);
		return S_FALSE;
	}

	if (!(CheckResult(InitWireframeView())
		&& CheckResult(InitSolidView()))) {
		MessageBox(_hWnd, L"Initialization of rasterizer failed", L"Error", MB_ICONERROR);
		return S_FALSE;
	}

	InitConstantBuffer();

	InitViewport();

	InitializeSampler();

	return S_OK;
}

void Graphics::InitializeSampler()
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	_pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);
}

HRESULT Graphics::InitRenderTarget()
{
	HRESULT hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		return hr;

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	pBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

	// Set primitive topology
	_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Graphics::Cleanup()
{
	if (_pImmediateContext) _pImmediateContext->ClearState();
	if (_pConstantBuffer) _pConstantBuffer->Release();

	if (_pSamplerLinear) _pSamplerLinear->Release();

	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pSwapChain) _pSwapChain->Release();
	if (_pImmediateContext) _pImmediateContext->Release();
	if (_pd3dDevice) _pd3dDevice->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();
	if (_wireFrame) _wireFrame->Release();
	if (_solid) _solid->Release();

	mCurrentCamera = nullptr;
}


HRESULT Graphics::CreateTexture(const wchar_t* filepath, ID3D11ShaderResourceView** texture)
{
	return CreateDDSTextureFromFile(_pd3dDevice, filepath, nullptr, texture);
}

ID3D11Device* Graphics::GetDevice()
{
	return _pd3dDevice;
}

ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return _pImmediateContext;
}

int Graphics::GetMouseX()
{
	return mMouseX;
}

int Graphics::GetMouseY()
{
	return mMouseY;
}

void Graphics::SwitchVertexBuffer(ID3D11Buffer* buffer)
{
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	_pImmediateContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}

void Graphics::SwitchIndexBuffer(ID3D11Buffer* buffer)
{
	_pImmediateContext->IASetIndexBuffer(buffer, DXGI_FORMAT_R16_UINT, 0);
}

HRESULT Graphics::InitDepthBuffer()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = _WindowWidth;
	depthStencilDesc.Height = _WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
	return _pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);
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
	sd.BufferDesc.Width = _WindowWidth;
	sd.BufferDesc.Height = _WindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
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
	return _pd3dDevice->CreateRasterizerState(&wfdesc, &_wireFrame);
}

HRESULT Graphics::InitSolidView()
{
	D3D11_RASTERIZER_DESC sodesc;
	ZeroMemory(&sodesc, sizeof(D3D11_RASTERIZER_DESC));
	sodesc.FillMode = D3D11_FILL_SOLID;
	sodesc.CullMode = D3D11_CULL_BACK;
	return _pd3dDevice->CreateRasterizerState(&sodesc, &_solid);
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
	return _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);
}

void Graphics::InitViewport()
{
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_WindowWidth;
	vp.Height = (FLOAT)_WindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &vp);
}

bool Graphics::CheckResult(int in)
{
	if (in < 0) { return false; }
	return true;
}

void Graphics::ClearBuffers()
{
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, clearColor);
	
	_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphics::Draw(unsigned int indexCount)
{
	//Draws object to swap chain buffer
	_pImmediateContext->DrawIndexed(indexCount, 0, 0);
}

void Graphics::Present()
{
	// Present our back buffer to our front buffer
	_pSwapChain->Present(0, 0);
}

void Graphics::BindTextures(int startSlot, int count, std::vector<ID3D11ShaderResourceView*> textures)
{
	_pImmediateContext->PSSetShaderResources(startSlot, count, &textures[0]);
}

void Graphics::ClearTextures()
{
	_pImmediateContext->PSSetShaderResources(0, 0, nullptr);
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
	GetClientRect(_hWnd, &rect);
	MapWindowPoints(_hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Graphics::FreeCursor()
{
	ClipCursor(nullptr);
}


void Graphics::SwitchCamera(Camera* newCamera)
{
	mCurrentCamera = newCamera;
}

void Graphics::EnableWireframe(bool enabled)
{
	ID3D11RasterizerState* renderState;
	enabled ? renderState = _wireFrame : renderState = _solid;
	_pImmediateContext->RSSetState(renderState);
}

void Graphics::SetShaders(ID3D11VertexShader* vs, ID3D11PixelShader* ps)
{
	_pImmediateContext->VSSetShader(vs, nullptr, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetShader(ps, nullptr, 0);
	_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);
}

void Graphics::SetInputLayout(ID3D11InputLayout* layout)
{
	_pImmediateContext->IASetInputLayout(layout);
}

UINT Graphics::GetWindowWidth()
{
	return _WindowWidth;
}

UINT Graphics::GetWindowHeight()
{
	return _WindowHeight;
}

void Graphics::UpdateBuffers(XMFLOAT4X4& position)
{
	ConstantBuffer cb;
	cb.mView = XMMatrixTranspose(mCurrentCamera->View());
	cb.mProjection = XMMatrixTranspose(mCurrentCamera->Proj());
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&position));

	cb.DiffuseMtrl = diffuseLight.material;
	cb.DiffuseLight = diffuseLight.light;
	cb.AmbientLight = ambientLight.light;
	cb.AmbientMtrl = ambientLight.material;
	cb.SpecularMtrl = specularLight.material;
	cb.SpecularPower = specularLight.intensity;
	cb.SpecularLight = specularLight.light;
	cb.LightVec3 = lightDirection;

	cb.EyePosW = mCurrentCamera->GetPosition();

	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
}