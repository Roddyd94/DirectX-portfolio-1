#pragma once
#include "Types.h"
#include "Core/Object.h"

class AIState : public Object
{
    friend class AIComponent;

public:
    AIState()           = default;
    ~AIState() override = default;

protected:
    std::function<void()> _callback[AIEventState::End];
    std::string           _name;

    std::vector<Ptr<class AITransition>> _transitions;

public:
    virtual bool Init(const std::string& name);
    void         Destroy() override;

    virtual Ptr<AIState> Tick(float deltaTime);

    const std::string& GetName() const;

    void SetName(const std::string& name);

    virtual void OnEnter();
    virtual void OnExit();
    virtual void OnTick();

public:
    template <typename T>
    Ptr<T> CreateAITransition(const std::string& name, Ptr<AIState> jumpState, TransitionRule rule)
    {
        auto transition = FindAITransition<T>(name);
        if (transition)
            return transition;

        transition = New<T>();
        if (!transition->Init(name, jumpState, rule))
        {
            DESTROY(transition);
            return nullptr;
        }

        _transitions.push_back(transition);

        return transition;
    }

    template <typename T>
    void AddCallback(AIEventState::Type stateType, T&& func)
    {
        _callback[stateType] = std::forward<T>(func);
    }

    template <typename T>
    void AddCallback(AIEventState::Type stateType, T* obj, void (T::*memFunc)())
    {
        _callback[stateType] = std::bind(memFunc, obj);
    }

    template <typename T>
    void AddCondition(const std::string& name, T&& func)
    {
        for (auto& transition : _transitions)
        {
            if (transition->GetName() == name)
            {
                transition->AddCondition(func);
                break;
            }
        }
    }

    template <typename T>
    void AddCondition(const std::string& name, T* obj, bool (T::*memFunc)())
    {
        for (auto& transition : _transitions)
        {
            if (transition->GetName() == name)
            {
                transition->AddCondition(obj, memFunc);
                break;
            }
        }
    }

    template <typename T>
    Ptr<T> FindAITransition(const std::string& name)
    {
        for (auto& transition : _transitions)
        {
            if (transition->GetName() == name)
                return transition;
        }

        return nullptr;
    }
};
