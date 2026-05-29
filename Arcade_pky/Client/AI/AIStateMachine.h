#pragma once
#include "AIBlackboard.h"
#include "AICondition.h"
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
    Ptr<class AIBlackboard> _blackboard;
    Ptr<class AIState>      _currentState = nullptr;

    float _accTime = 0.f;

protected:
    virtual void Init(Ptr<class AIComponent> owner);

public:
    void Destroy() override;

    void Tick(float deltaTime);
    void Transition(const std::string& stateName);

    Ptr<class AIComponent> GetOwner() const;
    Ptr<class AIState>     GetCurrentState() const;
    Ptr<class AIState>     FindAIState(const std::string& name) const;

    Ptr<class AIState>      CreateAIState(const std::string& name);
    Ptr<class AITransition> CreateAITransition(const std::string& stateName,
      const std::string&                                          transitionName,
      const std::string&                                          jumpStateName,
      Ptr<AIConditionBase>                                        condition) const;

public:
    template <typename T>
    Ptr<T> GetAIBlackboard() const
    {
        return Cast<AIBlackboard, T>(_blackboard);
    }

    template <typename T>
    Ptr<T> GetCurrentState() const
    {
        return Cast<AIState, T>(_currentState);
    }

    template <typename T>
    Ptr<T> FindAIState(const std::string& name) const
    {
        return Cast<AIState, T>(FindAIState(name));
    }

    template <typename T, typename... Args>
    Ptr<T> CreateAIState(const std::string& name, Args&&... args)
    {
        auto state = FindAIState<T>(name);
        if (nullptr != state)
            return Cast<AIState, T>(state);

        state = New<T>(std::forward<Args>(args)...);
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
    Ptr<T> CreateBlackboard()
    {
        if (_blackboard)
            return Cast<AIBlackboard, T>(_blackboard);

        _blackboard = New<T>();

        return Cast<AIBlackboard, T>(_blackboard);
    }

    template <typename... Args>
    Ptr<class AIConditionBase> CreateAICondition(
      const std::string& conditionName, ConditionOperator op, Args&&... funcs)
    {
        Ptr<AICondition> condition = New<AICondition>();
        condition->SetName(conditionName);
        condition->SetOperator(op);
        (condition->AddCondition(std::forward<Args>(funcs)), ...);

        return condition;
    }

    template <typename... Args>
    Ptr<class AIConditionBase> CreateAICompositeCondition(
      const std::string& conditionName, ConditionOperator op, Args... conditions)
    {
        Ptr<AICompositeCondition> condition = New<AICompositeCondition>();
        condition->SetName(conditionName);
        condition->SetOperator(op);
        (condition->AddCondition(conditions), ...);

        return condition;
    }
};
