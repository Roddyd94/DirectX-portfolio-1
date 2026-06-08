#pragma once
#include "Core/StructureBuffer.h"

struct IndexedTileInstanceData
{
    Vector2 uvLT;
    Vector2 uvRB;
    Vector2 worldPos;
    Vector2 size;
    int32   paletteNumber;
};

class IndexedTileStructureBuffer : public StructureBuffer
{
public:
    IndexedTileStructureBuffer()           = default;
    ~IndexedTileStructureBuffer() override = default;

private:
    std::vector<IndexedTileInstanceData> _dataArray;

public:
    void Update() override;

    int32 GetElementCount() const override;
    void  Clear() override;

    void AddData(Vector2 worldPos, Vector2 uvLT, Vector2 uvRB, Vector2 size, int32 paletteNumber);
};
