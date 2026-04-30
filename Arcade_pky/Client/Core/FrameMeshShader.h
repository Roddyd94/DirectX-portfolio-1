#pragma once
#include "GraphicShader.h"

class FrameMeshShader : public GraphicShader
{
public:
    FrameMeshShader()           = default;
    ~FrameMeshShader() override = default;

public:
    bool Init() override;
};
