#pragma once
#include "SnowballMorphableEnemyBlackboard.h"

class GoblinBlackboard : public SnowballMorphableEnemyBlackboard
{
public:
    GoblinBlackboard()           = default;
    ~GoblinBlackboard() override = default;

public:
    void Destroy() override;
};
