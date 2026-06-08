#pragma once
#include "Core/GraphicShader.h"

class IndexedTileInstanceShader : public GraphicShader
{
public:
    IndexedTileInstanceShader()           = default;
    ~IndexedTileInstanceShader() override = default;

public:
    bool Init() override;
};
