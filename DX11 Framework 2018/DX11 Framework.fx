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
    float time;
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
VS_OUTPUT VS( float4 Pos : POSITION, float4 Color : COLOR)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    Pos.xy += 0.5f * sin(Pos.x) * sin(3.0f * time);
    
    Pos.z *= 0.6f + 0.4f * sin(2.0f * time);
    
    //converts from model to world space
    output.Pos = mul( Pos, World );
    //convers to camera space from world space
    output.Pos = mul( output.Pos, View );
    
    //converts from view space to projection
    output.Pos = mul( output.Pos, Projection );
    output.Color = Color;
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
