#include "pch.h"

#include "MaterialConstantBuffer.h"

void MaterialConstantBuffer::Update()
{
    SetData(&_data);
}

void MaterialConstantBuffer::SetBaseColor(Vector4 v)
{
    SetBaseColor(v.r, v.g, v.b, v.a);
}

void MaterialConstantBuffer::SetBaseColor(float r, float g, float b, float a)
{
    _data.baseColor.r = r;
    _data.baseColor.g = g;
    _data.baseColor.b = b;
    _data.baseColor.a = a;
}

void MaterialConstantBuffer::SetBaseColor(Color color)
{
    _data.baseColor = color;
}

void MaterialConstantBuffer::SetOpacity(float opacity)
{
    _data.baseColor.a = opacity;
}

void MaterialConstantBuffer::SetTextureWidth(int32 width)
{
    _data.textureWidth = width;
}

void MaterialConstantBuffer::SetTextureHeight(int32 height)
{
    _data.textureHeight = height;
}
