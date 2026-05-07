struct PS_Output_Single
{
    float4 color : SV_TARGET;
};

// VERTEX CONSTANT BUFFER
cbuffer Transform : register(b0)
{
    matrix gWorld;
    matrix gView;
    matrix gProj;
    matrix gWv;
    matrix gWvp;
}
// !VERTEX CONSTANT BUFFER

// PIXEL CONSTANT BUFFER
cbuffer FrameMesh : register(b0)
{
    float4 gFrameLineColor;
}
// !PIXEL CONSTANT BUFFER

// GLOBAL SAMPLER AND TEXTURE
SamplerState gBaseSampler : register(s0);
Texture2D gBaseTexture : register(t0);
// !GLOBAL SAMPLER AND TEXTURE

float2 UpdateSpriteAtlasUV(float2 uv, float2 uvLT, float2 uvRB, int flipX = 0)
{
    float2 result = (float2) 0.f;
    
    if (flipX == 1)
        uv.x = 1 - uv.x;
    
    if (uv.x < 0.5f)
        result.x = uvLT.x;
    else
        result.x = uvRB.x;
    
    if (uv.y < 0.5f)
        result.y = uvLT.y;
    else
        result.y = uvRB.y;
    
    return result;
}
