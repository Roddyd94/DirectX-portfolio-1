#include "pch.h"

#include "TileOutlineStructureBuffer.h"

void TileOutlineStructureBuffer::Update()
{
    if (_dataArray.empty())
        return;

    int32 count = GetElementCount();
    Resize(count);
    SetData(_dataArray.data(), count);
}

int32 TileOutlineStructureBuffer::GetElementCount() const
{
    return static_cast<int32>(_dataArray.size());
}

void TileOutlineStructureBuffer::Clear()
{
    _dataArray.clear();
}

void TileOutlineStructureBuffer::AddData(
  Vector4 color, Vector2 worldPos, Vector2 size)
{
    TileOutlineInstanceData data;
    data.color    = color;
    data.worldPos = worldPos;
    data.size     = size;

    _dataArray.push_back(data);
}
