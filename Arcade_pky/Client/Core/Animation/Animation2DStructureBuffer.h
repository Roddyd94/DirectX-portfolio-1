#pragma once
#include "Core/StructureBuffer.h"

struct Animation2DInstanceData
{
    Vector2 uvLT;
    Vector2 uvRB;
    Vector2 worldPos;
    Vector2 size;
    int     flipX;
};

class Animation2DStructureBuffer : public StructureBuffer
{
public:
    Animation2DStructureBuffer()           = default;
    ~Animation2DStructureBuffer() override = default;

private:
    std::vector<Animation2DInstanceData> _dataArray;

public:
    void Update() override;

    int32 GetElementCount() const override;
    void  Clear() override;
    
    void AddData(const Animation2DInstanceData& data);
    void AddData(Animation2DInstanceData&& data);
};
