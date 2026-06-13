#include "IndexedTexture.fx"

cbuffer SpriteData : register(b1)
{
    float2 uvLT;
    float2 uvRB;
    int paletteNumber;
    int flipX;
};

struct VS_Input
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
    int paletteNumber : TEXCOORD1;
};

VS_Output IndexedSpriteVS(VS_Input input)
{
    VS_Output output = (VS_Output) 0;
    
    float3 pos = input.pos;
    
    output.pos = mul(float4(pos, 1.f), gWvp);
    output.uv = UpdateSpriteAtlasUV(input.uv, uvLT, uvRB, flipX);
    output.paletteNumber = paletteNumber;
    
    return output;
}

PS_Output_Single IndexedSpritePS(VS_Output input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.color = GetColorFromUV(input.paletteNumber, input.uv);
    
    return output;
}