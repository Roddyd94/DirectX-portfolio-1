#pragma once
#include "Object.h"

class SubManager : public Object
{
public:
    SubManager()           = default;
    ~SubManager() override = default;

public:
    virtual bool Init();
};
