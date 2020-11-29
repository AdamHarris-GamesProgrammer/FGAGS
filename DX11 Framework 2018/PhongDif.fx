//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "LightUtilities.fx"
#include "VertexShaderUtilities.fx"

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);

SamplerState samplerAnisotropic : register(s0);


cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    
    Material objectMaterial;
    DirectionalLight directionalLight;
    float Padding;
    float3 EyePosW;
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    CalculateVSOutput(World, View, Projection, input, output);
    output.PosW = normalize(EyePosW.xyz - output.Pos.xyz);
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 textureColour = txDiffuse.Sample(samplerAnisotropic, input.Tex);
    
    if (textureColour.a < 0.01f)
    {
        discard;
    }
    
    input.normalW = normalize(input.normalW);
    
    float3 toEyeW = normalize(EyePosW - input.PosW);
    
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 A, D, S;
    
    CalculateDirectionalLight(objectMaterial, directionalLight, input.normalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    
    float4 litColor = textureColour * (ambient + diffuse) + spec;
    litColor.a = objectMaterial.Diffuse.a;
    
    return litColor;
}
