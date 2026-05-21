#include "pch.h"

#include "MonkeyStateMachine.h"

#include "MonkeyBlackboard.h"

void MonkeyStateMachine::Init(Ptr<class AIComponent> owner)
{
    AIStateMachine::Init(owner);
    CreateBlackboard<MonkeyBlackboard>();
}

void MonkeyStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}
