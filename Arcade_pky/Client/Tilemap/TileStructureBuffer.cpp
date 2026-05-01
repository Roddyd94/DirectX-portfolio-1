#include "pch.h"

#include "TileStructureBuffer.h"

void TileStructureBuffer::Update()
{
    if (_dataArray.empty())
        return;

    int32 count = GetElementCount();
    Resize(count);
    SetData(_dataArray.data(), count);
}

int32 TileStructureBuffer::GetElementCount() const
{
    return static_cast<int32>(_dataArray.size());
}

void TileStructureBuffer::Clear()
{
    _dataArray.clear();
}

void TileStructureBuffer::AddData(
  Vector2 worldPos, Vector2 uvLT, Vector2 uvRB, Vector2 size)
{
    TileInstanceData data;
    data.worldPos = worldPos;
    data.uvLT     = uvLT;
    data.uvRB     = uvRB;
    data.size     = size;

    _dataArray.push_back(data);
}
