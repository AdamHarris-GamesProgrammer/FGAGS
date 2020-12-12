#include "LightUtilities.fx"
#include "VertexShaderUtilities.fx"
#include "ConstantBuffers.fx"

// Texture Maps for Diffuse, Specular and Normal
Texture2D txDiffuse : register(t0);
Texture2D txSpecular : register(t1);
Texture2D txNormal : register(t2);

//Sampler
SamplerState samplerAnisotropic : register(s0);

// Pixel Shader
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 textureColour = txDiffuse.Sample(samplerAnisotropic, input.Tex);
    
	clip(textureColour.a - 0.25f);
    
    
    float4 specularColour = txSpecular.Sample(samplerAnisotropic, input.Tex);
    
    float3 normalColour = txNormal.Sample(samplerAnisotropic, input.Tex);
    
    input.normalW = normalize(input.normalW);
    
    float3 toEyeW = normalize(EyePosW - input.PosW);
    
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 A, D, S;
    
	float3 norm = input.normalW * normalColour;
    
    CalculateDirectionalLight(objectMaterial, directionalLight, norm, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;
    
    CalculatePointLight(objectMaterial, pointLight, input.PosW, norm, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

	CalculateSpotLight(objectMaterial, spotLight, input.PosW, norm, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec += S;
    
    float4 litColor = textureColour * (ambient + diffuse) + (specularColour * spec);
    litColor.a = objectMaterial.Diffuse.a;
    
    return litColor;
}
