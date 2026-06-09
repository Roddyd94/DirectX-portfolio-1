#pragma once
#include "AITransition.h"
#include "Types.h"
#include "Core/Object.h"

class AIState : public Object
{
    friend class AIComponent;

public:
    AIState()           = default;
    ~AIState() override = default;

protected:
    std::function<void(float)> _callback[AIEventState::End];
    std::string                _name;

    std::unordered_map<std::string, size_t> _transitionFinder;
    std::vector<Ptr<class AITransition>>    _transitions;

    float _interval    = 0.f;
    bool  _hasInterval = false;

public:
    virtual bool Init(const std::string& name);
    void         Destroy() override;

    virtual void OnEnter(float deltaTime);
    virtual void OnExit(float deltaTime);
    virtual void OnTick(float deltaTime);

    Ptr<AIState> Tick(float deltaTime);

    const std::string& GetName() const;

    float GetInterval() const;
    bool  HasInterval() const;

    Ptr<class AITransition> FindAITransition(const std::string& name) const;

    void SetName(const std::string& name);
    void SetInterval(float interval);
    void SetCondition(const std::string& transitionName, Ptr<class AIConditionBase> condition);
    
    Ptr<class AITransition> CreateAITransition(
      const std::string& name, Ptr<AIState> jumpState, Ptr<AIConditionBase> condition);
    
public:
    template <typename T>
    void RegisterCallback(AIEventState::Type stateType, T&& func)
    {
        _callback[stateType] = std::forward<T>(func);
    }

    template <typename T>
    void RegisterCallback(AIEventState::Type stateType, T* obj, void (T::*memFunc)())
    {
        _callback[stateType] = std::bind(memFunc, obj, std::placeholders::_1);
    }
};
