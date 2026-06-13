#include "Share.fx"

cbuffer IndexedTextureInfo : register(b0)
{
    uint2 textureSize;
    uint bitsPerPixel;
    uint paletteLength; // 2 ^ bitsPerPixel
}

Texture2D<uint> gIndexedTexture : register(t0);

StructuredBuffer<float4> paletteData : register(t1);

float4 GetColorFromUV(int paletteNumber, float2 uv)
{
    uint2 coords = uv * textureSize;
    uint2 byteCoords = coords;
    
    int pixelsPerByte = 8 / bitsPerPixel;
    byteCoords.x /= pixelsPerByte;
    
    uint loadData = gIndexedTexture.Load(int3(byteCoords, 0));
    uint colorIndex = 0;
    
    if (bitsPerPixel == 2)
    {
        uint shift = (3 - (coords.x & 3)) * 2;
        colorIndex = (loadData >> shift) & 0x3;
    }
    else if (bitsPerPixel == 4)
    {
        colorIndex = (coords.x & 1) ? (loadData & 0xF) : (loadData >> 4);
        colorIndex &= 0xF;
    }
    else if (bitsPerPixel == 8)
    {
        colorIndex &= 0xFF;
    }
    
    return paletteData[paletteLength * paletteNumber + colorIndex];
}