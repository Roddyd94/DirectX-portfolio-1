#pragma once
#include "AI/AIStateMachine.h"

class MonkeyStateMachine : public AIStateMachine
{
public:
    MonkeyStateMachine()           = default;
    ~MonkeyStateMachine() override = default;

protected:
    void Init(Ptr<class AIComponent> owner) override;

public:
    void Destroy() override;
};
