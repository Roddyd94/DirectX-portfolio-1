#pragma once
#include "SnowballMorphableEnemyStateMachine.h"

class MonkeyStateMachine : public SnowballMorphableEnemyStateMachine
{
public:
    MonkeyStateMachine()           = default;
    ~MonkeyStateMachine() override = default;

protected:
    void Init(Ptr<class AIComponent> owner) override;

public:
    void Destroy() override;
};
