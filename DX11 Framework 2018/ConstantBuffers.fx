#ifndef CONSTANT_BUFFERS_FX
#define CONSTANT_BUFFERS_FX

#include "LightStructures.fx"

//Constant buffer, this is stored in a seperate file to allow each shader file to have access to it. it also ensures one point of failure if there is a issue with the constant buffer
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