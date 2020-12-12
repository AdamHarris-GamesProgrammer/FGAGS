#ifndef LIGHT_FX
#define LIGHT_FX

struct Light
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

struct DirectionalLight : Light
{
    float3 Direction;
    float Padding;
};

struct PointLight : Light
{
    float3 Position;
    float Range;
    
    float3 Attenuation;
    float Padding;
};

struct SpotLight : Light
{
    float3 Position;
    float Range;
    
    float3 Direction;
    float Spot;
    
    float3 Attenuation;
    float Padding;
};

struct Material : Light
{
    float4 Reflect;
};

#endif