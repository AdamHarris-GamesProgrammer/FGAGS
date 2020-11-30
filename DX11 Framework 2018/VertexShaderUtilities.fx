struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 PosW : POSITION;
    float3 normalW : NORMAL;
    float2 Tex : TEXCOORD0;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
};

VS_OUTPUT CalculateVSOutput(matrix World, matrix View, matrix Projection, float3 EyePosW, VS_INPUT input, out VS_OUTPUT output)
{
    output = (VS_OUTPUT) 0;
    //converts from model to world space
    output.Pos = mul(float4(input.Pos, 1.0f),World);
    
    output.PosW = mul(float4(input.Pos, 1.0f), World);
    
    //convers to camera space from world space
    output.Pos = mul(output.Pos, View);
    
    //converts from view space to projection
    output.Pos = mul(output.Pos, Projection);
    
    //multiplies the normal position with the world position to then get the normal 
    float3 normalW = mul(float4(input.Normal, 0.0f), World).xyz;
    output.normalW = normalize(normalW);

    output.Tex = input.Tex;
    
    return output;
}