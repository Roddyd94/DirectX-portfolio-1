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

// BossAnimation struct

class BossStateMachine : public AIStateMachine
{
public:
    BossStateMachine()           = default;
    ~BossStateMachine() override = default;

public:
    bool Init(Ptr<class AIComponent> owner) override;
    void Destroy() override;

    void ChangeLowerBody(SnowbrosBossLowerFrameType type);
};
