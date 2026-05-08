#pragma once
#include "Core/StructureBuffer.h"

struct TileOutlineInstanceData
{
    Vector4 color;
    Vector2 worldPos;
    Vector2 size;
};

class TileOutlineStructureBuffer : public StructureBuffer
{
public:
    TileOutlineStructureBuffer()           = default;
    ~TileOutlineStructureBuffer() override = default;

private:
    std::vector<TileOutlineInstanceData> _dataArray;

public:
    void Update() override;

    int32 GetElementCount() const override;
    void  Clear() override;

    void AddData(Vector4 color, Vector2 worldPos, Vector2 size);
};
