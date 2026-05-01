#pragma once
#include "Core/GraphicShader.h"

class TileOutlineInstanceShader : public GraphicShader
{

public:
    TileOutlineInstanceShader()           = default;
    ~TileOutlineInstanceShader() override = default;

public:
    bool Init() override;
};
