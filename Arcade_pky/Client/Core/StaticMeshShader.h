#pragma once
#include "GraphicShader.h"

class StaticMeshShader : public GraphicShader
{
public:
    StaticMeshShader()           = default;
    ~StaticMeshShader() override = default;

public:
    bool Init() override;
    void Destroy() override;
};
