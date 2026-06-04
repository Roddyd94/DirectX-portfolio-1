#pragma once
#include "Core/InstanceRenderer.h"

class SpriteInstanceRenderer : public InstanceRenderer
{
public:
    SpriteInstanceRenderer()           = default;
    ~SpriteInstanceRenderer() override = default;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
};
