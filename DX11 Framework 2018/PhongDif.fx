#include "LightUtilities.fx"
#include "VertexShaderUtilities.fx"
#include "ConstantBuffers.fx"

//Diffuse Texture Map
Texture2D txDiffuse : register(t0);

//Sampler
SamplerState samplerAnisotropic : register(s0);

// Pixel Shader
float4 PS(VS_OUTPUT input) : SV_Target
{
    //Samples the texture colour from the diffuse map at this texel
    float4 textureColour = txDiffuse.Sample(samplerAnisotropic, input.Tex);
    
    //clips the texture value is the alpha is less than 0.25. Saves processing time for near invisible opacities
    clip(textureColour.a - 0.25f);
    
    
    //Calculates the vector from the cameras position (EyePosW) and the objects position (input.PosW)
    float3 toEyeW = normalize(EyePosW - input.PosW);
    
    //variables used to store the final colours of each types of shading
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //short hand for the normal at this texel of the normal map
    float3 norm = input.normalW;
    
    //Creates temporary variables to store the ambient (A), diffuse (D) and specular (S) colours from each stage
    float4 A, D, S;
    CalculateDirectionalLight(objectMaterial, directionalLight, norm, toEyeW, A, D, S);
    //Adds the directional light effects to the final colours
    ambient += A;
    diffuse += D;
    spec += S;
    
    CalculatePointLight(objectMaterial, pointLight, input.PosW, norm, toEyeW, A, D, S);
    //Adds the point light effects to the final colours
    ambient += A;
    diffuse += D;
    spec += S;

    CalculateSpotLight(objectMaterial, spotLight, input.PosW, norm, toEyeW, A, D, S);
    //Adds the spot light effects to the final colours
    ambient += A;
    diffuse += D;
    spec += S;
    
    //Calculates the final colour based on the texture colour and the calculated lighting colours
    float4 litColor = textureColour * (ambient + diffuse) + spec;
    //Sets the alpha colour
    litColor.a = objectMaterial.Diffuse.a;
    
    //Returns the final colour
    return litColor;
}
