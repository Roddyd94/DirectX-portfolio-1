#include "pch.h"

#include "AIState.h"

#include "AITransition.h"

bool AIState::Init(const std::string& name)
{
    _name = name;

    return true;
}

void AIState::Destroy()
{
    for (auto& transition : _transitions)
        DESTROY(transition)
}

Ptr<AIState> AIState::Tick(float deltaTime)
{
    for (auto& transition : _transitions)
    {
        if (transition->CheckCondition(deltaTime))
            return transition->GetJumpState();
    }

    return This<AIState>();
}

const std::string& AIState::GetName() const
{
    return _name;
}

void AIState::SetName(const std::string& name)
{
    _name = name;
}

void AIState::OnEnter()
{
    if (_callback[AIEventState::Enter])
        _callback[AIEventState::Enter]();
}

void AIState::OnExit()
{
    if (_callback[AIEventState::Exit])
        _callback[AIEventState::Exit]();
}

void AIState::OnTick()
{
    if (_callback[AIEventState::Tick])
        _callback[AIEventState::Tick]();
}
