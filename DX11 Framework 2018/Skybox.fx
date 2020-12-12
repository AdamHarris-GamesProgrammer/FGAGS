#include "VertexShaderUtilities.fx"

//Diffuse Texture Map
Texture2D txDiffuse : register(t0);

//Sampler
SamplerState samplerAnisotropic : register(s0);

//Pixel Shader
float4 PS(VS_OUTPUT input) : SV_Target
{
    //The Skybox shader only needs to return the colour of the sampled texel at the specified location
    return txDiffuse.Sample(samplerAnisotropic, input.Tex);
}
