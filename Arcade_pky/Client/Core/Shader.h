#pragma once
#include "Resource.h"

class Shader : public Resource
{
public:
    Shader()           = default;
    ~Shader() override = default;

public:
    virtual bool Init()      = 0;
    virtual void SetShader() = 0;
};
