#pragma once
#include "StructureBuffer.h"

struct TileLineInstanceData
{
    Vector4 color;
    Vector2 worldPos;
    Vector2 size;
};

class TileLineStructureBuffer : public StructureBuffer
{
public:
    TileLineStructureBuffer()           = default;
    ~TileLineStructureBuffer() override = default;

private:
    std::vector<TileLineInstanceData> _dataArray;

public:
    void Update() override;

    uint32 GetElementCount() const override;
    void   Clear() override;

    void AddData(Vector4 color, Vector2 worldPos, Vector2 size);
};
