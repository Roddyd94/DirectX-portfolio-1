#pragma once
#include "Core/GraphicShader.h"

class SpriteInstanceShader : public GraphicShader
{
public:
    SpriteInstanceShader()           = default;
    ~SpriteInstanceShader() override = default;

public:
    bool Init() override;
    void Destroy() override;
};
