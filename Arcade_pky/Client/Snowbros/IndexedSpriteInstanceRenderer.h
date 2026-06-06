#pragma once
#include "Core/InstanceRenderer.h"

class IndexedSpriteInstanceRenderer : public InstanceRenderer
{
public:
    IndexedSpriteInstanceRenderer()           = default;
    ~IndexedSpriteInstanceRenderer() override = default;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;

    void SetTexture(Ptr<class IndexedTexture> texture);
};
