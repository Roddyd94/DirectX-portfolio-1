#include "pch.h"

#include "IndexedAnimation2DConstantBuffer.h"

void IndexedAnimation2DConstantBuffer::Update()
{
    BindData(&_data);
}

void IndexedAnimation2DConstantBuffer::SetData(
  Vector2 uvLT, Vector2 uvRB, int32 paletteNumber, bool flipX)
{
    _data.uvLT          = uvLT;
    _data.uvRB          = uvRB;
    _data.paletteNumber = paletteNumber;
    _data.flipX         = flipX;
}
