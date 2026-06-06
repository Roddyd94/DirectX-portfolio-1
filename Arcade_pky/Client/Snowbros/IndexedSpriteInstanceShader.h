#pragma once
#include "Core/GraphicShader.h"

class IndexedSpriteInstanceShader : public GraphicShader
{
public:
    IndexedSpriteInstanceShader()           = default;
    ~IndexedSpriteInstanceShader() override = default;

public:
    bool Init() override;
    void Destroy() override;
};
