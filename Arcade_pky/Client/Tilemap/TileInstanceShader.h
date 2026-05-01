#pragma once
#include "Core/GraphicShader.h"

class TileInstanceShader : public GraphicShader
{
public:
    TileInstanceShader()           = default;
    ~TileInstanceShader() override = default;

public:
    bool Init() override;
};
