#include "Share.fx"

ByteAddressBuffer textureIndexBytes : register(t0);

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
    uint textureStrideBitSize;
    uint paletteLength; // 2 ^ textureStrideBitSize
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

float4 GetColorFromIndex(int paletteNumber, float2 uv)
{
    uint2 coords = uv * textureSize;

    int pixelCountsPerByte = 8 / textureStrideBitSize;
    int textureIndex = coords.y * (textureSize.x / pixelCountsPerByte) + coords.x / pixelCountsPerByte;
    int loadIndex = textureIndex - textureIndex % 4;
    uint pixelDataPerFourByte = textureIndexBytes.Load(loadIndex);

    uint dataPosition = textureIndex % 4;
    uint paletteIndex = pixelDataPerFourByte >> (dataPosition) * 8;
    
    if (textureStrideBitSize == 2)
    {
        paletteIndex >>= textureStrideBitSize * (pixelCountsPerByte - 1 - coords.x % pixelCountsPerByte);
        paletteIndex &= 0x3;
    }
    else if (textureStrideBitSize == 4)
    {
        paletteIndex >>= textureStrideBitSize * (pixelCountsPerByte - 1 - coords.x % pixelCountsPerByte);
        paletteIndex &= 0xF;
    }
    else if (textureStrideBitSize == 8)
    {
        paletteIndex &= 0xFF;
    }
    
    return paletteData[paletteLength * paletteNumber + paletteIndex];
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
    
    output.color = GetColorFromIndex(input.paletteNumber, input.uv);
    
    return output;
}