#include "Share.fx"

struct VS_Input_Tex
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_Output_Tex
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

cbuffer Animation2D : register(b1)
{
    float2 uvLT;
    float2 uvRB;
    int flipX;
}

VS_Output_Tex SpriteVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    
    float3 pos = input.pos;
    
    output.pos = mul(float4(pos, 1.f), gWvp);
    output.uv = UpdateSpriteAtlasUV(input.uv, uvLT, uvRB, flipX);
    
    return output;
}


PS_Output_Single SpritePS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.color = gBaseTexture.Sample(gBaseSampler, input.uv);
    
    return output;
}
