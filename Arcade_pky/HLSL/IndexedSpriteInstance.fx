#include "Share.fx"

ByteAddressBuffer pixelIndexBytes : register(t0);

struct SpriteInstanceData
{
    float2 uvLT;
    float2 uvRB;
    float2 worldPos;
    float2 size;
    int paletteNumber;
    int flipX;
};
StructuredBuffer<SpriteInstanceData> spriteInstances : register(t1);
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

VS_Output IndexedSpriteInstanceVS(VS_Input input, uint instanceID : SV_InstanceID)
{
    VS_Output output = (VS_Output) 0;
    
    SpriteInstanceData instance = spriteInstances[instanceID];
    
    float3 pos = input.pos;
    pos.x = pos.x * instance.size.x + instance.worldPos.x;
    pos.y = pos.y * instance.size.y + instance.worldPos.y;
    pos.z = 1.f;
    
    output.pos = mul(float4(pos, 1.f), gWvp);
    output.uv = UpdateSpriteAtlasUV(input.uv, instance.uvLT, instance.uvRB, instance.flipX);
    output.paletteNumber = instance.paletteNumber;
    
    return output;
}

PS_Output_Single IndexedSpriteInstancePS(VS_Output input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    output.color = GetColorFromUV(input.paletteNumber, input.uv);
    
    return output;
}