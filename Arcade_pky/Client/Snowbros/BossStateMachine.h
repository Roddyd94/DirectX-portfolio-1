#pragma once
#include "AI/AIStateMachine.h"

enum class SnowbrosBossAnimationType
{
    Stand,
    Shout,
    Hop,
    Jump,
    Fall,
    Dead,
    None
};

enum class SnowbrosBossLowerFrameType
{
    Low,
    Mid,
    High,
    None
};

class BossStateMachine : public AIStateMachine
{
public:
    BossStateMachine()           = default;
    ~BossStateMachine() override = default;

public:
    bool Init(Ptr<class AIComponent> owner) override;
    void Destroy() override;

    void ChangeLowerBody(SnowbrosBossLowerFrameType type);
    void Hit(int32 damage);
    void Jump();
    void SpawnSpawn(bool spawnOnLeft);
    void AdjustLowerBody();
};
