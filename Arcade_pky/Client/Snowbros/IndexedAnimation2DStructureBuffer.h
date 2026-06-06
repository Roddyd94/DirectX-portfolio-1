#pragma once
#include "Core/StructureBuffer.h"

struct IndexedAnimation2DInstanceData
{
    Vector2 uvLT;
    Vector2 uvRB;
    Vector2 worldPos;
    Vector2 size;
    int32   paletteNumber;
    int32   flipX;
};

class IndexedAnimation2DStructureBuffer : public StructureBuffer
{
public:
    IndexedAnimation2DStructureBuffer()           = default;
    ~IndexedAnimation2DStructureBuffer() override = default;

private:
    std::vector<IndexedAnimation2DInstanceData> _dataArray;

public:
    void Update() override;

    int32 GetElementCount() const override;
    void  Clear() override;

    void AddData(const IndexedAnimation2DInstanceData& data);
    void AddData(IndexedAnimation2DInstanceData&& data);
};
