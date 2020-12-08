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

void CalculateDirectionalLight(Material mat, DirectionalLight light, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = -light.Direction;
    
    ambient = mat.Ambient * light.Ambient;
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
        specular = specFactor * mat.Specular * light.Specular;
    }
}

void CalculatePointLight(Material mat, PointLight light, float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 lightVec = light.Position - pos;
    
    //Distance from Surface to light
    float d = length(lightVec);
    
    
    if (d > light.Range)
        return;
    
    lightVec /= d;
    
    ambient = mat.Ambient * light.Ambient;
    
    float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specularFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
        diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
        specular = specularFactor * mat.Specular * light.Specular;
    }
    
    float att = 1.0f / dot(light.Attenuation, float3(1.0f, d, d * d));
    
    diffuse *= att;
    specular *= att;
}

void CalculateSpotLight(Material mat, SpotLight light, float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = light.Position - pos;
    
	float d = length(lightVec);
    
	if (d > light.Range)
		return;
    
	lightVec /= d;
    
	ambient = mat.Ambient * light.Ambient;
    
	float diffuseFactor = dot(lightVec, normal);
    
    [flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
        
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
		specular = specFactor * mat.Specular * light.Specular;
	}
       
	float spot = pow(max(dot(-lightVec, light.Direction), 0.0f), light.Spot);
    
	float att = spot / dot(light.Attenuation, float3(1.0f, d, d * d));
    
	ambient *= spot;
	diffuse *= att;
	specular *= att;

}