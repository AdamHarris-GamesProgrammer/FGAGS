#include "Graphics.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
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
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	_pCubeVertexBuffer = nullptr;
	_pCubeIndexBuffer = nullptr;
	_pConstantBuffer = nullptr;

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


	// Initialize the view matrix
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&_view, XMMatrixLookAtLH(Eye, At, Up));

	// Initialize the projection matrix
	XMStoreFloat4x4(&_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, _WindowWidth / (FLOAT)_WindowHeight, 0.01f, 100.0f));

	//Initialise the time class
	//time = Time();

	lightDirection = XMFLOAT3(0.25f, 0.5f, -1.0f);

	diffuseMaterial = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);

	diffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	ambientLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.2f);
	ambientMaterial = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);

	specularMtrl = { 0.0f,0.8f,0.0f,1.0f };
	specularLight = {0.5f,0.5f,0.5f, 1.0f };
	specularPower = 10.0f;
	eyePos = Eye;

	return S_OK;
}

HRESULT Graphics::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
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
		&& CheckResult(InitRenderTarget())
		&& CheckResult(InitShadersAndInputLayout())
		)) {
		MessageBox(_hWnd, L"Initialization of device failed", L"Error", MB_ICONERROR);
		return S_FALSE;
	}

	if (!(CheckResult(InitWireframeView())
		&& CheckResult(InitSolidView()))) {
		MessageBox(_hWnd, L"Initialization of rasterizer failed", L"Error", MB_ICONERROR);
		return S_FALSE;
	}

	if (!(CheckResult(InitVertexBuffer())
		&& CheckResult(InitIndexBuffer())
		&& CheckResult(InitConstantBuffer())
		)) {
		MessageBox(_hWnd, L"Initialization of buffers failed", L"Error", MB_ICONERROR);
	}

	InitViewport();

	return S_OK;
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
	if (_pCubeVertexBuffer) _pCubeVertexBuffer->Release();
	if (_pCubeIndexBuffer) _pCubeIndexBuffer->Release();
	if (_pVertexLayout) _pVertexLayout->Release();
	if (_pVertexShader) _pVertexShader->Release();
	if (_pPixelShader) _pPixelShader->Release();
	if (_pRenderTargetView) _pRenderTargetView->Release();
	if (_pSwapChain) _pSwapChain->Release();
	if (_pImmediateContext) _pImmediateContext->Release();
	if (_pd3dDevice) _pd3dDevice->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();
	if (_wireFrame) _wireFrame->Release();
	if (_solid) _solid->Release();
}

HRESULT Graphics::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

		if (pErrorBlob) pErrorBlob->Release();

		return hr;
	}

	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

HRESULT Graphics::InitShadersAndInputLayout()
{
	HRESULT hr;

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(L"DX11 Framework.fx", "VS", "vs_4_0", &pVSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"DX11 Framework.fx", "PS", "ps_4_0", &pPSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

	if (FAILED(hr))
		return hr;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &_pVertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
		return hr;

	// Set the input layout
	_pImmediateContext->IASetInputLayout(_pVertexLayout);

	return hr;
}

HRESULT Graphics::InitVertexBuffer()
{
	InitCubeVertexBuffer();
	InitPyramidVertexBuffer();

	GeneratePlane(3.0f, 3.0f, 6, 4);

	return S_OK;
}

void Graphics::InitCubeVertexBuffer()
{
	// Create vertex buffer
	SimpleVertex cubeVertices[] =
	{
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f)	,XMFLOAT3(-1.0f,-1.0f,-1.0f)	}, //Bottom Back Left
		{ XMFLOAT3(1.0f,-1.0f,-1.0f)	,XMFLOAT3(1.0f,-1.0f,-1.0f)	}, //Bottom Back Right
		{ XMFLOAT3(-1.0f,1.0f,-1.0f)	,XMFLOAT3(-1.0f,1.0f,-1.0f)	}, //Top Back Left
		{ XMFLOAT3(1.0f,1.0f,-1.0f)		,XMFLOAT3(1.0f,1.0f,-1.0f)	}, //Top Back Right
		{ XMFLOAT3(-1.0f,-1.0f,1.0f)	,XMFLOAT3(-1.0f,-1.0f,1.0f)	}, //Bottom Front Left
		{ XMFLOAT3(1.0f,-1.0f,1.0f)		,XMFLOAT3(1.0f,-1.0f,1.0f)	}, //Bottom Front Right
		{ XMFLOAT3(-1.0f,1.0f,1.0f)		,XMFLOAT3(-1.0f,1.0f,1.0f)	}, //Top Front Left
		{ XMFLOAT3(1.0f,1.0f,1.0f)		,XMFLOAT3(1.0f,1.0f,1.0f)	}  //Top Front Right
	};

	//Sets up the buffer description
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(cubeVertices) * 36;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;



	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = cubeVertices;

	_pd3dDevice->CreateBuffer(&bd, &InitData, &_pCubeVertexBuffer);
}

void Graphics::InitPyramidVertexBuffer()
{
	SimpleVertex pyramidVertices[] =
	{
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f)	,XMFLOAT3(0.0f,0.0f,1.0f)	},
		{ XMFLOAT3(-1.0f,-1.0f,1.0f)	,XMFLOAT3(0.0f,1.0f,0.0f)	},
		{ XMFLOAT3(1.0f,-1.0f,1.0f)		,XMFLOAT3(1.0f,0.0f,1.0f)	},
		{ XMFLOAT3(1.0f,-1.0f,-1.0f)	,XMFLOAT3(1.0f,1.0f,0.0f)	},
		{ XMFLOAT3(0.0f,1.0f,0.0f)		,XMFLOAT3(1.0f,1.0f,1.0f)	},
	};

	D3D11_BUFFER_DESC pbd;
	ZeroMemory(&pbd, sizeof(pbd));
	pbd.Usage = D3D11_USAGE_DEFAULT;
	pbd.ByteWidth = sizeof(pyramidVertices);
	pbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	pbd.CPUAccessFlags = 0;



	D3D11_SUBRESOURCE_DATA pInitData;
	ZeroMemory(&pInitData, sizeof(pInitData));
	pInitData.pSysMem = pyramidVertices;

	_pd3dDevice->CreateBuffer(&pbd, &pInitData, &_pPyramidVertexBuffer);
}

ID3D11Buffer* Graphics::GeneratePlaneVertices(float width, float depth, int rows, int columns)
{
	std::vector<SimpleVertex> planeVerts;
	ID3D11Buffer* buffer;

	UINT vertexCount = rows * columns;
	UINT faceCount = (rows - 1) * (columns - 1) * 2;
	//
	// Create the vertices.
	//
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;
	float dx = width / (columns - 1);
	float dz = depth / (rows - 1);
	float du = 1.0f / (columns - 1);
	float dv = 1.0f / (rows - 1);
	planeVerts.resize(vertexCount);
	for (UINT i = 0; i < rows; ++i)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < columns; ++j)
		{
			float x = -halfWidth + j * dx;
			planeVerts[i * columns + j].Pos = XMFLOAT3(x, 0.0f, z);
			planeVerts[i * columns + j].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
		}

	}

	//__debugbreak();

	D3D11_BUFFER_DESC plbd;
	ZeroMemory(&plbd, sizeof(plbd));
	plbd.Usage = D3D11_USAGE_DEFAULT;
	plbd.ByteWidth = sizeof(planeVerts) * (planeVerts.size() * 2);
	plbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	plbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA plInitData;
	ZeroMemory(&plInitData, sizeof(plInitData));
	plInitData.pSysMem = &planeVerts[0];

	_pd3dDevice->CreateBuffer(&plbd, &plInitData, &buffer);

	return buffer;
}

ID3D11Buffer* Graphics::GeneratePlaneIndices(int height, int width)
{
	std::vector<WORD> planeIndices;
	ID3D11Buffer* buffer;

	planeIndices.resize(((height * width) * 2) * 3);
	UINT k = 0;
	for (UINT i = 0; i < height - 1; ++i)
	{
		for (UINT j = 0; j < width - 1; ++j)
		{
			planeIndices[k] = i * width + j;
			planeIndices[k + 1] = i * width + j + 1;
			planeIndices[k + 2] = (i + 1) * width + j;
			planeIndices[k + 3] = (i + 1) * width + j;
			planeIndices[k + 4] = i * width + j + 1;
			planeIndices[k + 5] = (i + 1) * width + j + 1;
			k += 6; // next quad
		}
	}


	//__debugbreak();

	D3D11_BUFFER_DESC plbd;
	ZeroMemory(&plbd, sizeof(plbd));

	plbd.Usage = D3D11_USAGE_DEFAULT;
	plbd.ByteWidth = sizeof(planeIndices) * planeIndices.size();
	plbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	plbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA plInitData;

	ZeroMemory(&plInitData, sizeof(plInitData));
	plInitData.pSysMem = &planeIndices[0];
	_pd3dDevice->CreateBuffer(&plbd, &plInitData, &buffer);

	return buffer;
}

void Graphics::GeneratePlane(float width, float depth, int rows, int columns)
{
	_pPlaneVertexBuffer = GeneratePlaneVertices(width, depth, rows, columns);
	_pPlaneIndexBuffer = GeneratePlaneIndices(rows, columns);
}

HRESULT Graphics::InitIndexBuffer()
{
	InitCubeIndexBuffer();
	InitPyramidIndexBuffer();

	return S_OK;
}

void Graphics::InitCubeIndexBuffer()
{
	// Create index buffer
	WORD cubeIndices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(cubeIndices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = cubeIndices;
	_pd3dDevice->CreateBuffer(&bd, &InitData, &_pCubeIndexBuffer);
}

void Graphics::InitPyramidIndexBuffer()
{
	WORD pyramidIndices[] = {
	0,1,4,
	1,2,4,
	2,3,4,
	3,0,4,
	0,3,1,
	1,3,2
	};
	D3D11_BUFFER_DESC pbd;
	ZeroMemory(&pbd, sizeof(pbd));
	pbd.Usage = D3D11_USAGE_DEFAULT;
	pbd.ByteWidth = sizeof(pyramidIndices);
	pbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	pbd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA pInitData;
	ZeroMemory(&pInitData, sizeof(pInitData));
	pInitData.pSysMem = pyramidIndices;
	_pd3dDevice->CreateBuffer(&pbd, &pInitData, &_pPyramidIndexBuffer);
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

void Graphics::SetPyramidBuffer()
{
	SwitchVertexBuffer(_pPyramidVertexBuffer);
	SwitchIndexBuffer(_pPyramidIndexBuffer);
}

void Graphics::SetCubeBuffer()
{
	SwitchVertexBuffer(_pCubeVertexBuffer);
	SwitchIndexBuffer(_pCubeIndexBuffer);
}

void Graphics::SetPlaneBuffer()
{
	SwitchVertexBuffer(_pPlaneVertexBuffer);
	SwitchIndexBuffer(_pPlaneIndexBuffer);
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

void Graphics::EnableWireframe(bool enabled)
{
	ID3D11RasterizerState* renderState;
	enabled ? renderState = _wireFrame : renderState = _solid;
	_pImmediateContext->RSSetState(renderState);
}

void Graphics::SetShaders()
{
	_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
}

void Graphics::UpdateBuffers(XMFLOAT4X4& position, float t)
{
	ConstantBuffer cb;
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&_view));
	cb.mProjection = XMMatrixTranspose(XMLoadFloat4x4(&_projection));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&position));
	cb.DiffuseMtrl = diffuseMaterial;
	cb.DiffuseLight = diffuseLight;
	cb.LightVec3 = lightDirection;
	cb.AmbientLight = ambientLight;
	cb.AmbientMtrl = ambientMaterial;
	cb.SpecularMtrl = specularMtrl;
	cb.SpecularPower = specularPower;
	cb.SpecularLight = specularLight;
	cb.EyePosW = { eyePos.m128_f32[0], eyePos.m128_f32[1], eyePos.m128_f32[2] };

	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
}