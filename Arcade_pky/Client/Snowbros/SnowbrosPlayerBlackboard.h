#pragma once
#include "Core/Types.h"
#include "Player/PlayerBlackboard.h"

class SnowbrosPlayerBlackboard : public PlayerBlackboard
{
public:
    SnowbrosPlayerBlackboard()           = default;
    ~SnowbrosPlayerBlackboard() override = default;

public:
    std::map<ComponentIDPair, Weak<class CollisionComponent>> overlappedSnowballs;
    float                                                     speedX = 2.f;
};
