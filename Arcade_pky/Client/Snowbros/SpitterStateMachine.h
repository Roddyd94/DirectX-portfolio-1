#pragma once
#include "SnowballMorphableEnemyStateMachine.h"

class SpitterStateMachine : public SnowballMorphableEnemyStateMachine
{
public:
    SpitterStateMachine()           = default;
    ~SpitterStateMachine() override = default;

protected:
    void Init(Ptr<class AIComponent> owner) override;

public:
    void Destroy() override;
};
