#pragma once
#include "Core/Animation/SpriteInstanceComponent.h"

class IndexedSpriteInstanceComponent : public SpriteInstanceComponent
{
public:
    IndexedSpriteInstanceComponent()           = default;
    ~IndexedSpriteInstanceComponent() override = default;

protected:
    int32 _paletteNumber = 0;

public:
    void AddBufferData() override;

    void SetPaletteNumber(int32 number);
};
