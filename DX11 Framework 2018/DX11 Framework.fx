//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    
    float4 DiffuseMtrl;
    float4 DiffuseLight;
    float3 LightVecW;
}

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION, float3 Normal : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    

    
    //converts from model to world space
    output.Pos = mul( Pos, World );
    
    //convers to camera space from world space
    output.Pos = mul( output.Pos, View );
    
    //converts from view space to projection
    output.Pos = mul( output.Pos, Projection );
    
    //multiplies the normal position with the world position to then get the normal 
    float3 normalW = mul(float4(Normal, 0.0f), World).xyz;
    //returns a value between 0 and 1
    normalW = normalize(normalW); 
    
    //calculates the diffuse amount from the max value of either the light vector or the normal 
    float diffuseAmount = max(dot(LightVecW, normalW), 0.0f);
    //sets the output colour based of the material and lighting colour 
    output.Color.rgb = diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb;
    //sets the alpha value to that of the material
    output.Color.a = DiffuseMtrl.a;
    
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    //returns the input colour from the passed in input which is the output of the vertex shader
    return input.Color;
}
