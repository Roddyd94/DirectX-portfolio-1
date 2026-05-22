#pragma once
#include "Core/Object.h"
class SnowballBlackboard : public Object
{
public:
    SnowballBlackboard()           = default;
    ~SnowballBlackboard() override = default;

public:
    float accValue  = 0.2f;
    bool  isOnFloor = false;

public:
    void Destroy() override;
};
