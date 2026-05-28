#pragma once
#include "SnowballMorphableEnemyStateMachine.h"

class GoblinStateMachine : public SnowballMorphableEnemyStateMachine
{
public:
    GoblinStateMachine()           = default;
    ~GoblinStateMachine() override = default;

protected:
    void Init(Ptr<class AIComponent> owner) override;

public:
    void Destroy() override;
};
