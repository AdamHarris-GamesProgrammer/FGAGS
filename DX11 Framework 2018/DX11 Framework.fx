//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    
    float4 DiffuseMtrl;
    float4 DiffuseLight;
    float4 AmbientMtrl;
    float4 AmbientLight;
    float3 LightVecW;
    float padding;
    float4 SpecularMtrl;
    float4 SpecularLight;
    float SpecularPower;
    float3 padding2;
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
VS_OUTPUT VS( float4 Pos : POSITION, float3 Normal : NORMAL, float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    
    //converts from model to world space
    output.Pos = mul( Pos, World );
    
    output.eye = normalize(EyePosW.xyz - output.Pos.xyz);
    
    //convers to camera space from world space
    output.Pos = mul( output.Pos, View );
    
    //converts from view space to projection
    output.Pos = mul( output.Pos, Projection );
    
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
float4 PS( VS_OUTPUT input ) : SV_Target
{   
    float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);
    
    //Get each pixels normal
    input.normalW = normalize(input.normalW);
    
    //compute reflection vector
    float r = reflect(-LightVecW, input.normalW);
    // Compute Colour using Diffuse lighting only
    float diffuseAmount = max(dot(LightVecW, input.normalW), 0.0f);
    // Determine how much (if any) specular light makes it into the eye.
    float specularAmount = pow(max(dot(r, input.eye), 0), SpecularPower);

    //specular calc
    float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;
    //ambient calc
    float3 ambient = AmbientMtrl * AmbientLight;
    //diffuse calc
    float3 diffuse = diffuseAmount * (textureColour * DiffuseMtrl * DiffuseLight).rgb;

    float4 finalColor;
    finalColor.rgb = clamp(diffuse, 0, 1) + ambient + clamp(specular, 0, 1);
    finalColor.a = DiffuseMtrl.a;

    return finalColor;
}
