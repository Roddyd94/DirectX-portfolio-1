#include "pch.h"

#include "TileLineStructureBuffer.h"

void TileLineStructureBuffer::Update()
{
    if (_dataArray.empty())
        return;

    uint32 count = GetElementCount();
    Resize(count);
    SetData(_dataArray.data(), count);
}

uint32 TileLineStructureBuffer::GetElementCount() const
{
    return static_cast<uint32>(_dataArray.size());
}

void TileLineStructureBuffer::Clear()
{
    _dataArray.clear();
}

void TileLineStructureBuffer::AddData(
  Vector4 color, Vector2 worldPos, Vector2 size)
{
    TileLineInstanceData data;
    data.color    = color;
    data.worldPos = worldPos;
    data.size     = size;

    _dataArray.push_back(data);
}
