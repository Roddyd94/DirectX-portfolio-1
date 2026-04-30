#pragma once
#include "Rendering.h"
#include "StructureBuffer.h"

struct TileInstanceData
{
    Vector2 uvLT;
    Vector2 uvRB;
    Vector2 worldPos;
    Vector2 size;
};

class TileStructureBuffer : public StructureBuffer
{
public:
    TileStructureBuffer()           = default;
    ~TileStructureBuffer() override = default;

private:
    std::vector<TileInstanceData> _dataArray;

public:
    void Update() override;

    uint32 GetElementCount() const override;
    void   Clear() override;

    void AddData(Vector2 worldPos, Vector2 uvLT, Vector2 uvRB, Vector2 size);
};
