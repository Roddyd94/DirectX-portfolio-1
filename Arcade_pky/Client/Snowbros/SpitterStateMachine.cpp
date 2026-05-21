#include "pch.h"

#include "SpitterStateMachine.h"

#include "SpitterBlackboard.h"

void SpitterStateMachine::Init(Ptr<class AIComponent> owner)
{
    AIStateMachine::Init(owner);
    CreateBlackboard<SpitterBlackboard>();
}

void SpitterStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}
