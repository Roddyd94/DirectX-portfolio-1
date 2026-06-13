#include "pch.h"

#include "Animation2DConstantBuffer.h"

void Animation2DConstantBuffer::Update()
{
    BindData(&_data);
}

void Animation2DConstantBuffer::SetFlipX(bool flipX)
{
    _data.flipX = flipX ? 1 : 0;
}

void Animation2DConstantBuffer::SetUV(Vector2 uvLT, Vector2 uvRB)
{
    _data.uvLT = uvLT;
    _data.uvRB = uvRB;
}

void Animation2DConstantBuffer::SetData(Vector2 uvLT, Vector2 uvRB, bool flipX)
{
    SetUV(uvLT, uvRB);
    SetFlipX(flipX);
}
