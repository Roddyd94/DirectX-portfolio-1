#include "Share.fx"

ByteAddressBuffer pixelIndexBytes : register(t0);

cbuffer SpriteData : register(b1)
{
    float2 uvLT;
    float2 uvRB;
    int paletteNumber;
    int flipX;
};
StructuredBuffer<float4> paletteData : register(t1);

cbuffer IndexedTextureInfo : register(b0)
{
    uint2 textureSize;
    uint bitsPerPixel;
    uint paletteLength; // 2 ^ bitsPerPixel
}

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

float4 GetColorFromUV(int paletteNumber, float2 uv)
{
    uint2 coords = uv * textureSize;

    int pixelsPerByte = 8 / bitsPerPixel;
    int pixelIndex = coords.y * (textureSize.x / pixelsPerByte) + coords.x / pixelsPerByte;
    int loadIndex = pixelIndex - pixelIndex % 4;
    uint loadData = pixelIndexBytes.Load(loadIndex);

    uint dataPosition = pixelIndex % 4;
    uint colorIndex = loadData >> (dataPosition) * 8;
    
    if (bitsPerPixel == 2)
    {
        colorIndex >>= bitsPerPixel * (pixelsPerByte - 1 - coords.x % pixelsPerByte);
        colorIndex &= 0x3;
    }
    else if (bitsPerPixel == 4)
    {
        colorIndex >>= bitsPerPixel * (pixelsPerByte - 1 - coords.x % pixelsPerByte);
        colorIndex &= 0xF; // 스트라이드 크기가 4-bit인 경우
    }
    else if (bitsPerPixel == 8)
    {
        colorIndex &= 0xFF;
    }
    
    return paletteData[paletteLength * paletteNumber + colorIndex];
}

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