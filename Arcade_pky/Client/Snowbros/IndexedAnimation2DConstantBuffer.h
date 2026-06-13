#pragma once
#include "Core/ConstantBuffer.h"

struct IndexedAnimation2DConstantBufferData
{
    Vector2 uvLT;
    Vector2 uvRB;
    int32   paletteNumber;
    int32   flipX;
    float   empty[2];
};

class IndexedAnimation2DConstantBuffer : public ConstantBuffer
{
public:
    IndexedAnimation2DConstantBuffer()           = default;
    ~IndexedAnimation2DConstantBuffer() override = default;

private:
    IndexedAnimation2DConstantBufferData _data;

public:
    void Update() override;
    void SetData(Vector2 uvLT, Vector2 uvRB, int32 paletteNumber, bool flipX);
};
