#pragma once
#include "AI/AIStateMachine.h"

class SpitterStateMachine : public AIStateMachine
{
public:
    SpitterStateMachine()           = default;
    ~SpitterStateMachine() override = default;

protected:
    void Init(Ptr<class AIComponent> owner) override;

public:
    void Destroy() override;
};
