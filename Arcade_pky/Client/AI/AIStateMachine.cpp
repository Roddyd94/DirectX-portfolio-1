#include "pch.h"

#include "AIStateMachine.h"

void AIStateMachine::Init(Ptr<class AIComponent> owner)
{
    _owner = owner;
}

void AIStateMachine::Destroy()
{
    DESTROY(_blackboard)

    for (auto& [_, state] : _states)
        DESTROY(state)

    _states.clear();
}

void AIStateMachine::Tick(float deltaTime)
{
    if (!_currentState)
        return;

    if (_currentState->HasInterval())
        _accTime += deltaTime;

    if (_accTime < _currentState->GetInterval())
        return;

    Ptr<AIState> state = _currentState->Tick(deltaTime);
    if (state)
    {
        if (state->GetName() == _currentState->GetName())
        {
            _currentState->OnTick(deltaTime);
        }
        else
        {
            _currentState->OnExit(deltaTime);
            _currentState = state;
            _currentState->OnEnter(deltaTime);
            _accTime = 0.f;
        }
    }

    _accTime -= _currentState->GetInterval();
}

void AIStateMachine::Transition(const std::string& stateName)
{
    auto state = FindAIState(stateName);
    _currentState->OnExit(0.f);
    _currentState = state;
    _currentState->OnEnter(0.f);
    _accTime = 0.f;
}

Ptr<class AIComponent> AIStateMachine::GetAIComponent() const
{
    return Lock(_owner);
}

Ptr<class AIState> AIStateMachine::FindAIState(const std::string& name) const
{
    auto it = _states.find(name);
    if (_states.end() == it)
        return nullptr;

    return it->second;
}

Ptr<class AIState> AIStateMachine::CreateAIState(const std::string& name)
{
    auto state = FindAIState(name);
    if (nullptr != state)
        return state;

    state = New<AIState>();
    if (false == state->Init(name))
    {
        DESTROY(state);
        return nullptr;
    }

    if (!_currentState)
        _currentState = state;

    _states[name] = state;

    return state;
}

Ptr<class AITransition> AIStateMachine::CreateAITransition(const std::string& stateName,
  const std::string&                                                          transitionName,
  const std::string&                                                          jumpStateName,
  Ptr<AIConditionBase>                                                        condition) const
{
    auto state = FindAIState(stateName);
    if (nullptr == state)
        return nullptr;

    auto jumpState = FindAIState(jumpStateName);

    return state->CreateAITransition(transitionName, jumpState, condition);
}
