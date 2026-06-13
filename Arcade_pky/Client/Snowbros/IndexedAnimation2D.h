#pragma once
#include "Core/Animation/Animation2D.h"

class IndexedAnimation2D : public Animation2D
{
public:
    IndexedAnimation2D()           = default;
    ~IndexedAnimation2D() override = default;

protected:
    int32 _paletteNumber;

public:
    bool Init() override;
    void BindConstantBuffer(Animation2DSprite& sprite) override;

    void SetPaletteNumber(int32 paletteNumber);
};
