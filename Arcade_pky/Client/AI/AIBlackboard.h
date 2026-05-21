#pragma once
#include "Core/Object.h"

class AIBlackboard : public Object
{
public:
    AIBlackboard()           = default;
    ~AIBlackboard() override = default;

public:
    void Destroy() override;

    virtual void SetDirection(float direction) = 0;
};
