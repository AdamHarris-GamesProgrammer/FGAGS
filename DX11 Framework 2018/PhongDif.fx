//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#include "LightUtilities.fx"

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
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 normalW : NORMAL;
    float3 eye : POSITION;
    float2 Tex : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    
    //converts from model to world space
    output.Pos = mul(Pos, World);
    
    output.eye = normalize(EyePosW.xyz - output.Pos.xyz);
    
    //convers to camera space from world space
    output.Pos = mul(output.Pos, View);
    
    //converts from view space to projection
    output.Pos = mul(output.Pos, Projection);
    
    //multiplies the normal position with the world position to then get the normal 
    float3 normalW = mul(float4(Normal, 0.0f), World).xyz;
    //returns a value between 0 and 1
    output.normalW = normalize(normalW);

    output.Tex = Tex;
    
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
    
    float3 toEyeW = normalize(EyePosW - input.eye);
    
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
