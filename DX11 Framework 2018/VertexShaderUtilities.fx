#include "ConstantBuffers.fx"

//Struct for the Vertex Shader Input
struct VS_INPUT
{
    float3 Pos : POSITION;  //Vertex Position
    float3 Normal : NORMAL; //Vertex Normal
    float2 Tex : TEXCOORD;  //Vertex Texture Coordinate
};

//Struct for the Vertex Shader output 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;   //Vertex Positon in screen space
    float3 PosW : POSITION;     //Vertex Positonm relative to the world
    float3 normalW : NORMAL;    //Vertex normals relative to world space
    float2 Tex : TEXCOORD0;     //Vertex texture coordinates. Unmodified from the input stage
};

//Vertex Shader takes the VS_INPUT struct for input and then returns a VS_OUTPUT struct
VS_OUTPUT VS(VS_INPUT input)
{
    //Initializes the VS_OUTPUT object
    VS_OUTPUT output;
    
    //Calculates the vertex position relative to the world matrix
    output.Pos = mul(float4(input.Pos, 1.0f), World);
    
    //Calculates the Vertex positon relative to world space
    output.PosW = output.Pos;
    
    //converts to camera space from world space
    output.Pos = mul(output.Pos, View);
    
    //converts from view space to projection
    output.Pos = mul(output.Pos, Projection);
    
    //Processes the vertex normals so they are relative to world space
    float3 normalW = mul(float4(input.Normal, 0.0f), World).xyz;
    //Normalizes the result
    output.normalW = normalize(normalW);

    //Sets the texture coordinate
    output.Tex = input.Tex;
    
    //Returns the output and sends it to the pixel shader
    return output;
}