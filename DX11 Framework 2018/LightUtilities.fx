#include "LightStructures.fx"

//This function calculates the effect the directional light has on the current pixel that is being processed
//Returns the ambient, diffuse and specular values using the out keyword
void CalculateDirectionalLight(Material mat, DirectionalLight light, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
    //Initializes the ambient, diffuse and specular values
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //Calculates the reversed light vector
    float3 lightVec = -light.Direction;
    
    //Calculates the ambient value based on the objects material and the ambient colour of the light
    ambient = mat.Ambient * light.Ambient;
    
    //Calculates the diffuse factor based on the light vector and the normal
    float diffuseFactor = dot(lightVec, normal);
    
    //Check to ensure this pixel needs to be processed. No point in processing it if the diffuse factor is 0.0 as it will come out black 
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        //calculates the reflection vector based on the light direction and the surface normal
        float3 v = reflect(-lightVec, normal);
        
        //Calculates the specular factor of the current pixel based on its location relative to the camera (toEye)
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        //Calculates the diffuse and specular colours based on the factors, material, and the light colours
        diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
        specular = specFactor * mat.Specular * light.Specular;
    }
}

void CalculatePointLight(Material mat, PointLight light, float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
    //Initializes the ambient, diffuse and specular values
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //Calculates the direction the light is travelling in from the position of the light and the object
    float3 lightVec = light.Position - pos;
    
    //Distance from Surface to light
    float d = length(lightVec);
    
    //if the distance from the light to the object is greater than the range of the light then return 
    if (d > light.Range)
        return;
    
    //Divide the light vector by the distance
    lightVec /= d;
    
    //Calculate the ambient colour
    ambient = mat.Ambient * light.Ambient;
    
    //Calculates the diffuse factor based on the light vector and the normal
    float diffuseFactor = dot(lightVec, normal);
    
    //Check to ensure this pixel needs to be processed. No point in processing it if the diffuse factor is 0.0 as it will come out black 
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        //calculates the reflection vector based on the light direction and the surface normal
        float3 v = reflect(-lightVec, normal);
        
        //Calculates the specular factor of the current pixel based on its location relative to the camera (toEye)
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        //Calculates the diffuse and specular colours based on the factors, material, and the light colours
        diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
        specular = specFactor * mat.Specular * light.Specular;
    }
    
    //Calculates the attenuation value 
    float att = 1.0f / dot(light.Attenuation, float3(1.0f, d, d * d));
    
    //Works out the final diffuse and specular values based off the attenuation
    diffuse *= att;
    specular *= att;
}

void CalculateSpotLight(Material mat, SpotLight light, float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
    //Initializes the ambient, diffuse and specular values
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //Calculates the direction the light is travelling in from the position of the light and the object
    float3 lightVec = light.Position - pos;
    
    //Distance from Surface to light
    float d = length(lightVec);
    
    //if the distance from the light to the object is greater than the range of the light then return 
    if (d > light.Range)
        return;
    
    //Divide the light vector by the distance
    lightVec /= d;
    
    //Calculate the ambient colour
    ambient = mat.Ambient * light.Ambient;
    
    //Calculates the diffuse factor based on the light vector and the normal
    float diffuseFactor = dot(lightVec, normal);
    
    //Check to ensure this pixel needs to be processed. No point in processing it if the diffuse factor is 0.0 as it will come out black 
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        //calculates the reflection vector based on the light direction and the surface normal
        float3 v = reflect(-lightVec, normal);
        
        //Calculates the specular factor of the current pixel based on its location relative to the camera (toEye)
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        //Calculates the diffuse and specular colours based on the factors, material, and the light colours
        diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
        specular = specFactor * mat.Specular * light.Specular;
    }
       
    //Calculates the spot factor of the light 
    float spot = pow(max(dot(-lightVec, light.Direction), 0.0f), light.Spot);
    
    //scales the attenuation by the spotlight factor
    float att = spot / dot(light.Attenuation, float3(1.0f, d, d * d));
    
    ambient *= spot;
    diffuse *= att;
    specular *= att;

}