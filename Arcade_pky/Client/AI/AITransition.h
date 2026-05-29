#pragma once
#include "AICondition.h"
#include "Types.h"
#include "Core/Object.h"

class AITransition : public Object
{
    friend class AIState;
    friend class AIComponent;

public:
    AITransition()           = default;
    ~AITransition() override = default;

protected:
    std::string                _name;
    Ptr<class AIConditionBase> _condition;
    Weak<class AIState>         _jumpState;

public:
    bool Init(const std::string& name, Ptr<class AIState> jumpState);
    void Destroy() override;

    const std::string& GetName() const;
    Ptr<class AIState> GetJumpState() const;

    void SetName(const std::string& name);
    void SetJumpState(Ptr<class AIState> state);
    void SetCondition(Ptr<class AIConditionBase> condition);

protected:
    virtual bool CheckCondition(float deltaTime);
};
