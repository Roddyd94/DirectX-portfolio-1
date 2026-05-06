#include "pch.h"

#include "AIStateMachine.h"

void AIStateMachine::Init(Ptr<class AIComponent> owner)
{
    _owner = owner;
}

void AIStateMachine::Destroy()
{
    DESTROY(_board)

    for (auto& [_, state] : _stateFinder)
        DESTROY(state)

    _stateFinder.clear();
}

void AIStateMachine::Tick(float deltaTime)
{
    if (!_currentState)
        return;

    _accTime += deltaTime;

    if (_accTime < _interval)
        return;

    Ptr<AIState> state = _currentState->Tick(deltaTime);
    if (state)
    {
        if (state->GetName() == _currentState->GetName())
        {
            _currentState->OnTick();
        }
        else
        {
            _currentState->OnExit();
            state->OnEnter();
            _currentState = state;

            OnStateChanged();
        }
    }

    _accTime -= _interval;
}

Ptr<class AIComponent> AIStateMachine::GetAIComponent() const
{
    return Lock(_owner);
}
