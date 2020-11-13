#pragma once

#include "d3d11.h"
#include "Graphics.h"

class Shaders
{
public:
	Shaders() = default;
	Shaders(Graphics* gfx);
	Shaders(Graphics* gfx, WCHAR* shaderPath);
	Shaders(Graphics* gfx, WCHAR* shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutArray, UINT layoutSize);

	ID3D11PixelShader* GetPixelShader();
	ID3D11VertexShader* GetVertexShader();
	D3D11_INPUT_ELEMENT_DESC* GetInputLayout();


	HRESULT InitializeShaders();
	void BindShaders();

private:
	Graphics* mGraphics;
	ID3D11Device* mDevice;
	D3D11_INPUT_ELEMENT_DESC* mLayoutDescription;
	ID3D11InputLayout* mLayout;

	UINT mLayoutSize;

	WCHAR* mShaderFile;

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
};

