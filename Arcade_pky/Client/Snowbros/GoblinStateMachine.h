#pragma once
#include "AI/AIStateMachine.h"

class GoblinStateMachine : public AIStateMachine
{
public:
    GoblinStateMachine()           = default;
    ~GoblinStateMachine() override = default;

protected:
    void Init(Ptr<class AIComponent> owner) override;

public:
    void Destroy() override;
};
