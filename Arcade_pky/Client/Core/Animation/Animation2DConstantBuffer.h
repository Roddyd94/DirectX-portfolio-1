#pragma once
#include "Core/ConstantBuffer.h"

struct Animation2DConstantBufferData
{
    Vector2 uvLT;
    Vector2 uvRB;
    int32   flipX;
    float   empty[3];
};

class Animation2DConstantBuffer : public ConstantBuffer
{
public:
    Animation2DConstantBuffer()           = default;
    ~Animation2DConstantBuffer() override = default;

private:
    Animation2DConstantBufferData _data;

public:
    void Update() override;
    void SetFlipX(bool flipX);
    void SetUV(Vector2 uvLT, Vector2 uvRB);
    void SetData(Vector2 uvLT, Vector2 uvRB, bool flipX);
};
