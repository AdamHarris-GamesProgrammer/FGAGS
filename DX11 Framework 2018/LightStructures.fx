#ifndef LIGHT_FX
#define LIGHT_FX

//Base light struct, all lights and materials have a Ambient, Diffuse and Specular colour
struct Light
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

//A directional light just requires a direction for the light to travel in 
struct DirectionalLight : Light
{
    float3 Direction;
    float Padding;
};

//A point light requires a position  of the lgiht and the range of the light, it also requires a attenuation factor to allow for a smooth decrease in intensity
struct PointLight : Light
{
    float3 Position;
    float Range;
    
    float3 Attenuation;
    float Padding;
};

//A spot light is similar to a point and direction light combined, in the way that it requires a position, direction, range and attenuation. However it also requires a spot factor
struct SpotLight : Light
{
    float3 Position;
    float Range;
    
    float3 Direction;
    float Spot;
    
    float3 Attenuation;
    float Padding;
};

//A material requires the three base colours (Ambient Diffuse and Specular). This struct is the same as the base light but is used for clarity
struct Material : Light
{
};

#endif