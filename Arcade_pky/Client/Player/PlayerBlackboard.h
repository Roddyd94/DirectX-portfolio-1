#pragma once
#include "Core/Object.h"

class PlayerBlackboard : public Object
{
public:
    PlayerBlackboard()           = default;
    ~PlayerBlackboard() override = default;

public:
    void Destroy() override;
};
