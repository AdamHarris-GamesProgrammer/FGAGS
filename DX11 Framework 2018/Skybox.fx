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
    PointLight pointLight;
    SpotLight spotLight;
    float3 EyePosW;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 textureColour = txDiffuse.Sample(samplerAnisotropic, input.Tex);
    
    return textureColour;
}
