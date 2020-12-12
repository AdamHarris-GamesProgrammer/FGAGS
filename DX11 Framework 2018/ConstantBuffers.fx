#ifndef CONSTANT_BUFFERS_FX
#define CONSTANT_BUFFERS_FX

#include "LightStructures.fx"

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

#endif 