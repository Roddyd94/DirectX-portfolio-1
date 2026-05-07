#include "pch.h"

#include "Animation2DConstantBuffer.h"

void Animation2DConstantBuffer::Update()
{
    SetData(&_data);
}

void Animation2DConstantBuffer::SetFlipX(bool flipX)
{
    _data.flipX = flipX ? 1 : 0;
}

void Animation2DConstantBuffer::SetUV(float ltx, float lty, float rbx, float rby)
{
    _data.uvLT.x = ltx;
    _data.uvLT.y = lty;
    _data.uvRB.x = rbx;
    _data.uvRB.y = rby;
}

void Animation2DConstantBuffer::SetUV(Vector2 uvLT, Vector2 uvRB)
{
    _data.uvLT = uvLT;
    _data.uvRB = uvRB;
}
