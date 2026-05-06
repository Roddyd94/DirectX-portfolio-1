#pragma once
#include "Core/Object.h"

class AIBoard : public Object
{
public:
    AIBoard()           = default;
    ~AIBoard() override = default;

public:
    void Destroy() override;
};
