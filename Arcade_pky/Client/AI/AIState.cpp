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
        if (transition && transition->CheckCondition(deltaTime))
            return transition->GetJumpState();
    }

    return This<AIState>();
}

const std::string& AIState::GetName() const
{
    return _name;
}

float AIState::GetInterval() const
{
    return _interval;
}

bool AIState::HasInterval() const
{
    return _hasInterval;
}

Ptr<AITransition> AIState::FindAITransition(const std::string& name) const
{
    auto it = _transitionFinder.find(name);

    if (_transitionFinder.end() == it)
        return nullptr;

    size_t index = it->second;

    if (index < 0 || index >= _transitions.size())
        return nullptr;

    auto transition = _transitions[index];
    if (nullptr == transition)
        return nullptr;

    if (transition->GetName() == name)
        return transition;

    return nullptr;
}

void AIState::SetName(const std::string& name)
{
    _name = name;
}

void AIState::SetInterval(float interval)
{
    _interval    = interval;
    _hasInterval = true;
}

void AIState::SetCondition(const std::string& transitionName, Ptr<class AIConditionBase> condition)
{
    auto transition = FindAITransition(transitionName);
    if (nullptr == transition)
        return;

    if (transition->GetName() != transitionName)
        return;

    transition->SetCondition(condition);
}

Ptr<class AITransition> AIState::CreateAITransition(
  const std::string& name, Ptr<AIState> jumpState, Ptr<AIConditionBase> condition)
{
    auto transition = FindAITransition(name);
    if (transition)
        return transition;

    transition = New<AITransition>();
    if (!transition->Init(name, jumpState))
    {
        DESTROY(transition);
        return nullptr;
    }

    transition->SetCondition(condition);

    _transitionFinder[name] = _transitions.size();
    _transitions.push_back(transition);

    return transition;
}

void AIState::OnEnter(float deltaTime)
{
    if (_callback[AIEventState::Enter])
        _callback[AIEventState::Enter](deltaTime);
}

void AIState::OnExit(float deltaTime)
{
    if (_callback[AIEventState::Exit])
        _callback[AIEventState::Exit](deltaTime);
}

void AIState::OnTick(float deltaTime)
{
    if (_callback[AIEventState::Tick])
        _callback[AIEventState::Tick](deltaTime);
}
