#include "pch.h"

#include "AITransition.h"

#include "AICondition.h"

bool AITransition::Init(const std::string& name, Ptr<class AIState> jumpState)
{
    _name      = name;
    _jumpState = jumpState;

    return true;
}

void AITransition::Destroy() {}

const std::string& AITransition::GetName() const
{
    return _name;
}

Ptr<class AIState> AITransition::GetJumpState() const
{
    return Lock(_jumpState);
}

void AITransition::SetName(const std::string& name)
{
    _name = name;
}

void AITransition::SetJumpState(Ptr<class AIState> state)
{
    _jumpState = state;
}

void AITransition::SetCondition(Ptr<class AIConditionBase> condition)
{
    _condition = condition;
}

bool AITransition::CheckCondition(float deltaTime)
{
    return _condition->CheckCondition(deltaTime);
}
