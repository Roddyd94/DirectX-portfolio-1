#include "pch.h"

#include "IndexedTextureInfoConstantBuffer.h"

void IndexedTextureInfoConstantBuffer::Update()
{
    SetData(&_data);
}

void IndexedTextureInfoConstantBuffer::SetSize(int32 width, int32 height)
{
    _data.textureWidth  = width;
    _data.textureHeight = height;
}

void IndexedTextureInfoConstantBuffer::SetStride(int32 strideBitSize)
{
    _data.textureStrideBitSize = strideBitSize;
    _data.paletteLength        = 1 << strideBitSize;
}
