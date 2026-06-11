#pragma once
#include "SnowballMorphableEnemyBlackboard.h"

class SpawnBlackboard : public SnowballMorphableEnemyBlackboard
{
public:
    float landPositionX = 1.f;
    float landPositionY = 1.f;

public:
    inline static Vector2 initialVelocity = {-10.f, -0.5f};

    inline static float dragX = 0.5f;
};
