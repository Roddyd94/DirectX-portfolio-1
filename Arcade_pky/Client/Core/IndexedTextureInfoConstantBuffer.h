#pragma once
#include "Core/ConstantBuffer.h"

struct IndexedTextureInfoConstantBufferData
{
    int32 textureWidth;
    int32 textureHeight;
    int32 textureStrideBitSize;
    int32 paletteLength;
};

class IndexedTextureInfoConstantBuffer : public ConstantBuffer
{
public:
    IndexedTextureInfoConstantBuffer()           = default;
    ~IndexedTextureInfoConstantBuffer() override = default;

private:
    IndexedTextureInfoConstantBufferData _data;

public:
    void Update() override;

    void SetSize(int32 width, int32 height);
    void SetStride(int32 strideBitSize);
};
