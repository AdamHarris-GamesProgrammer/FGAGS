#include "Shaders.h"

Shaders::Shaders(Graphics* gfx)
{
	mGraphics = gfx;
	mDevice = mGraphics->GetDevice();
}

Shaders::Shaders(Graphics* gfx, WCHAR* shaderPath)
{
	mGraphics = gfx;
	mDevice = mGraphics->GetDevice();
	mShaderFile = shaderPath;
}
								
Shaders::Shaders(Graphics* gfx, WCHAR* shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutArray, UINT layoutSize)
{
	mGraphics = gfx;
	mDevice = mGraphics->GetDevice();
	mShaderFile = shaderPath;
	mLayoutDescription = layoutArray;
	mLayoutSize = layoutSize;
}


HRESULT Shaders::InitializeShaders()
{
	HRESULT hr;

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(mShaderFile, "VS", "vs_4_0", &pVSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = mDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mVertexShader);

	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(mShaderFile, "PS", "ps_4_0", &pPSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = mDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &mPixelShader);
	pPSBlob->Release();

	if (FAILED(hr))
		return hr;

	// Create the input layout
	hr = mDevice->CreateInputLayout(mLayoutDescription, mLayoutSize, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &mLayout);
	pVSBlob->Release();

	if (FAILED(hr))
		return hr;

	return hr;
}

HRESULT Shaders::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
	hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
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

void Shaders::BindShaders()
{
	mGraphics->GetDeviceContext()->IASetInputLayout(mLayout);

	mGraphics->GetDeviceContext()->VSSetShader(mVertexShader, nullptr, 0);
	mGraphics->GetDeviceContext()->PSSetShader(mPixelShader, nullptr, 0);

	mGraphics->SetConstantBuffer();
}
#pragma region Getters

ID3D11PixelShader* Shaders::GetPixelShader()
{
	return mPixelShader;
}

ID3D11VertexShader* Shaders::GetVertexShader()
{
	return mVertexShader;
}

D3D11_INPUT_ELEMENT_DESC* Shaders::GetInputLayout()
{
	return mLayoutDescription;
}

#pragma endregion