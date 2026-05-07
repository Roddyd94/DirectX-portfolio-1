#pragma once
#include "Core/GraphicShader.h"

class SpriteShader : public GraphicShader
{
public:
    SpriteShader()           = default;
    ~SpriteShader() override = default;

public:
    bool Init() override;
    void Destroy() override;
};
