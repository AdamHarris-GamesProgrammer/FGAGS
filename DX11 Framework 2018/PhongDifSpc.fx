//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txSpecular : register(t1);


SamplerState samplerAnisotropic : register(s0);

struct Light
{
    float4 light;
    float4 lightColor;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    
    Light Diffuse;
    Light Ambient;
    Light Specular;
    float3 LightVecW;
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
    
    float4 specularColour = txSpecular.Sample(samplerAnisotropic, input.Tex);
    
    float3 specularVal = specularColour.xyz;
    float specularPower = specularColour.w;
    
    
    float3 normalW = normalize(input.normalW );
    float3 toEye = normalize(EyePosW - input.Pos);
    
    float3 lightLecNorm = normalize(LightVecW);
    
    //compute reflection vector
    float r = reflect(-lightLecNorm, normalW);
    // Compute Colour using Diffuse lighting only
    float diffuseAmount = max(dot(lightLecNorm, normalW), 0.0f);
    // Determine how much (if any) specular light makes it into the eye.
    float specularAmount = pow(max(dot(r, toEye), 0.0f), specularPower);

    //specular calc
    float3 specular = specularAmount * (specularVal * Specular.lightColor.rgb * Specular.light.rgb).rgb;
    //ambient calc
    float3 ambient = Ambient.lightColor * Ambient.light;
    //diffuse calc
    float3 diffuse = diffuseAmount * (textureColour * Diffuse.lightColor * Diffuse.light).rgb;

    float4 finalColor;
    finalColor.rgb = clamp(diffuse, 0, 1) + ambient + clamp(specular, 0, 1);
    finalColor.a = Diffuse.lightColor.a;

    return finalColor;
}
