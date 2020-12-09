#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {

public:
	VertexShader(ID3D11Device* device, ID3D11DeviceContext* context, WCHAR* shaderPath,
		D3D11_INPUT_ELEMENT_DESC* layoutArray, UINT layoutSize)
		: Bindable(device, context)
	{
		// Compile the vertex shader
		ID3DBlob* pVSBlob = nullptr;
		
		HRESULT hr;

		hr = CompileShaderFromFile(shaderPath, "VS", "vs_4_0", &pVSBlob);

		if (FAILED(hr))
		{
			MessageBox(nullptr,
				L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		}

		// Create the vertex shader
		
		hr = mDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &mVertexShader);

		if (FAILED(hr))
		{
			pVSBlob->Release();
		}

		mDevice->CreateInputLayout(layoutArray, layoutSize, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &mLayout);
		pVSBlob->Release();
	}

	void Bind() override
	{
		mContext->IASetInputLayout(mLayout);
		mContext->VSSetShader(mVertexShader, nullptr, 0);

	}

private:
	ID3D11VertexShader* mVertexShader;
	ID3D11InputLayout* mLayout;

	UINT mLayoutSize;


private:
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut) {
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

};

