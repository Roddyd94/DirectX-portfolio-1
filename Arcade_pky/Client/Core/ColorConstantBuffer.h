#pragma once
#include "ConstantBuffer.h"

struct ColorConstantBufferData
{
    Vector4 color;
};

class ColorConstantBuffer : public ConstantBuffer
{
public:
    ColorConstantBuffer()           = default;
    ~ColorConstantBuffer() override = default;

private:
    ColorConstantBufferData _data;

public:
    void Update() override;

    void SetData(float r, float g, float b, float a)
    {
        _data.color.r = r;
        _data.color.g = g;
        _data.color.b = b;
        _data.color.a = a;
    }
};
