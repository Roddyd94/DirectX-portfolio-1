#pragma once
#include "AI/AIState.h"

class SnowbrosEnemyState : public AIState
{
public:
    enum EnemyStateType;

public:
    SnowbrosEnemyState() = default;
    SnowbrosEnemyState(EnemyStateType stateType);
    ~SnowbrosEnemyState() override = default;

protected:
    EnemyStateType _stateType = End;

public:
    EnemyStateType GetStateType() const;

public:
    enum EnemyStateType
    {
        Stand,
        Patrol,
        Walk,
        Turn,
        Jump,
        Fall,
        Crouch,
        Dizzy,
        Struggle,
        Snowball,
        SnowballRolling,
        SnowballCrashing,
        Launched,
        Dead,
        Fire,
        Hop,
        Fly,
        End
    };
};
