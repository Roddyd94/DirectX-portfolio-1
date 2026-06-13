#pragma once
#include "Core/GraphicShader.h"

class IndexedSpriteShader : public GraphicShader
{
public:
    IndexedSpriteShader()           = default;
    ~IndexedSpriteShader() override = default;

public:
    bool Init() override;
    void Destroy() override;
};
