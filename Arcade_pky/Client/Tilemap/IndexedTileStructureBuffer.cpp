#include "pch.h"

#include "IndexedTileStructureBuffer.h"

void IndexedTileStructureBuffer::Update()
{
    if (_dataArray.empty())
        return;

    int32 count = GetElementCount();
    Resize(count);
    SetData(_dataArray.data(), count);
}

int32 IndexedTileStructureBuffer::GetElementCount() const
{
    return static_cast<int32>(_dataArray.size());
}

void IndexedTileStructureBuffer::Clear()
{
    _dataArray.clear();
}

void IndexedTileStructureBuffer::AddData(
  Vector2 worldPos, Vector2 uvLT, Vector2 uvRB, Vector2 size, int32 paletteNumber)
{
    IndexedTileInstanceData data;
    data.worldPos      = worldPos;
    data.uvLT          = uvLT;
    data.uvRB          = uvRB;
    data.size          = size;
    data.paletteNumber = paletteNumber;

    _dataArray.push_back(data);
}
