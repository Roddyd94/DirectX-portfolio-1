#include "Share.fx"

struct VS_Input
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer Material : register(b0)
{
    float4 materialBaseColor;
    int materialTextureWidth;
    int materialTextureHeight;
    float2 empty;
}

VS_Output StaticMeshVS(VS_Input input)
{
    VS_Output output = (VS_Output) 0;
    
    output.pos = mul(float4(input.pos, 1.f), gWvp);
    output.uv = input.uv;
    
    return output;
}

PS_Output_Single StaticMeshPS(VS_Output input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 color = gBaseTexture.Sample(gBaseSampler, input.uv);
    output.color = color * materialBaseColor;
    
    return output;
}