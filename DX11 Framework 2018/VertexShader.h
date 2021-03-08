#pragma once
#include "Bindable.h"

/// <summary>
/// Vertex Shader class used to create, store and bind a vertex shader to the graphical pipeline 
/// </summary>
class VertexShader : public Bindable {

public:
	//Creates a Vertex Shader object
	VertexShader(ID3D11Device* device, ID3D11DeviceContext* context)
		: Bindable(device, context)
	{
		ID3DBlob* pVSBlob = nullptr;
		
		HRESULT hr;

		// Compiles the vertex shader
		hr = CompileShaderFromFile(L"VertexShaderUtilities.fx", &pVSBlob);

		if (FAILED(hr))
		{
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		}

		// Create the vertex shader
		hr = pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);

		if (FAILED(hr))
		{
			pVSBlob->Release();
		}

		//Creates a temporary layout array for the vertex shader
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		//Creates the Input layout 
		pDevice->CreateInputLayout(layout, 3, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &pLayout);
		pVSBlob->Release();
	}

	//Binds the input layout and the vertex shader
	void Bind() override
	{
		pDeviceContext->IASetInputLayout(pLayout);
		pDeviceContext->VSSetShader(pVertexShader, nullptr, 0);
	}

	~VertexShader() {
		if (pVertexShader) pVertexShader->Release();
		if (pLayout) pLayout->Release();
	}

private:
	ID3D11VertexShader* pVertexShader;
	ID3D11InputLayout* pLayout;

	UINT mLayoutSize;


private:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, ID3DBlob** ppBlobOut) {
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
		hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_4_0",
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

};

