#pragma once
#include "AIBoard.h"
#include "AIState.h"
#include "AITransition.h"
#include "Core/Object.h"

class AIStateMachine : public Object
{
    friend class AIComponent;

public:
    AIStateMachine()           = default;
    ~AIStateMachine() override = default;

protected:
    std::unordered_map<std::string, Ptr<class AIState>> _states;

    Weak<class AIComponent> _owner;
    Ptr<class AIBoard>      _board;
    Ptr<class AIState>      _currentState;

    float _accTime  = 0.f;
    float _interval = 0.5f;

public:
    void Init(Ptr<class AIComponent> owner);
    void Destroy() override;

    void Tick(float deltaTime);

    Ptr<class AIComponent> GetAIComponent() const;

protected:
    virtual void OnStateChanged() = 0;

public:
    template <typename T>
    Ptr<T> GetAIBoard()
    {
        return Cast<AIBoard, T>(_board);
    }

    template <typename T>
    Ptr<T> FindAIState(const std::string& name)
    {
        auto it = _states.find(name);
        if (_states.end() == it)
            return nullptr;

        return it->second;
    }

    template <typename T>
    Ptr<T> CreateAIState(const std::string& name)
    {
        auto state = FindAIState<T>(name);
        if (nullptr != state)
            return state;

        state = New<T>();
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

    template <typename T>
    Ptr<T> CreateAITransition(const std::string& stateName,
      const std::string&                         tranName,
      const std::string&                         jumpStateName,
      TransitionRule                            rule)
    {
        auto state = FindAIState<AIState>(stateName);
        if (nullptr == state)
            return nullptr;

        auto jumpState = FindAIState<AIState>(jumpStateName);

        return state->CreateAITransition<T>(tranName, jumpState, rule);
    }

    template <typename T>
    Ptr<T> CreateAIBoard()
    {
        if (_board)
            return Cast<AIBoard, T>(_board);

        _board = New<T>();

        return Cast<AIBoard, T>(_board);
    }
};
