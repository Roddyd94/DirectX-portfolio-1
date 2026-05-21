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
    std::unordered_map<std::string, Ptr<class AIState>>         _states;
    std::unordered_map<std::string, Ptr<class AIConditionBase>> _conditions;

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

    Ptr<class AIComponent> GetAIComponent() const;
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
    Ptr<T> FindAICondition(const std::string& name) const
    {
        auto it = _conditions.find(name);
        if (_conditions.end() == it)
            return nullptr;

        return Cast<AIConditionBase, T>(it->second);
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

        _conditions[conditionName] = condition;

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

        _conditions[conditionName] = condition;

        return condition;
    }
};
