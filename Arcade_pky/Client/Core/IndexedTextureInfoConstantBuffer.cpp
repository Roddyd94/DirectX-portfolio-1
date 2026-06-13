#include "pch.h"

#include "IndexedTextureInfoConstantBuffer.h"

void IndexedTextureInfoConstantBuffer::Update()
{
    BindData(&_data);
}

void IndexedTextureInfoConstantBuffer::SetData(int32 width, int32 height, int32 bitsPerPixel)
{
    SetSize(width, height);
    SetBitsPerPixel(bitsPerPixel);
}

void IndexedTextureInfoConstantBuffer::SetSize(int32 width, int32 height)
{
    _data.textureWidth  = width;
    _data.textureHeight = height;
}

void IndexedTextureInfoConstantBuffer::SetBitsPerPixel(int32 bitsPerPixel)
{
    _data.textureStrideBitSize = bitsPerPixel;
    _data.paletteLength        = 1 << bitsPerPixel;
}
