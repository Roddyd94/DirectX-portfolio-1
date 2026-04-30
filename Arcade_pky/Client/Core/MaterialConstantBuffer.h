#pragma once
#include "ConstantBuffer.h"

struct MaterialConstantBufferData
{
    Vector4 baseColor;
    int32   textureWidth;
    int32   textureHeight;
    float   empty[2];
};

class MaterialConstantBuffer : public ConstantBuffer
{
public:
    MaterialConstantBuffer()           = default;
    ~MaterialConstantBuffer() override = default;

private:
    MaterialConstantBufferData _data;

public:
    virtual void Update();

    void SetBaseColor(Vector4 v);
    void SetBaseColor(float r, float g, float b, float a);
    void SetBaseColor(Color color);
    void SetOpacity(float opacity);
    void SetTextureWidth(int32 width);
    void SetTextureHeight(int32 height);
};
