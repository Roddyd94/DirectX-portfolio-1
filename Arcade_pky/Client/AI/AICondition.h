#pragma once
#include "Types.h"
#include "Core/Object.h"

class AIConditionBase : public Object
{
public:
    AIConditionBase()           = default;
    ~AIConditionBase() override = default;

protected:
    std::string       _name;
    ConditionOperator _operator;

public:
    void         Destroy() override;
    virtual bool CheckCondition(float deltaTime) const = 0;

    void SetName(const std::string& name);
    void SetOperator(ConditionOperator op);
};

class AICondition : public AIConditionBase
{
public:
    AICondition()           = default;
    ~AICondition() override = default;

private:
    std::vector<std::function<bool(float)>> _conditions;

public:
    bool CheckCondition(float deltaTime) const override;

public:
    template <typename T>
    void AddCondition(T&& func)
    {
        _conditions.push_back(std::forward<T>(func));
    }

    template <typename T>
    void AddCondition(T* obj, bool (T::*memFunc)(float))
    {
        _conditions.push_back(std::bind(memFunc, obj));
    }
};

class AICompositeCondition : public AIConditionBase
{
public:
    AICompositeCondition()           = default;
    ~AICompositeCondition() override = default;

private:
    std::vector<Ptr<AIConditionBase>> _conditions;

public:
    bool CheckCondition(float deltaTime) const override;
    void AddCondition(Ptr<AIConditionBase> condition);

public:
    template <typename T>
    void AddCondition(T&& func)
    {
        Ptr<AICondition> condition = New<AICondition>();
        condition->AddCondition(func);
        _conditions.push_back(condition);
    }

    template <typename T>
    void AddCondition(T* obj, bool (T::*memFunc)(float))
    {
        Ptr<AICondition> condition = New<AICondition>();
        condition->AddCondition(obj, memFunc);
        _conditions.push_back(condition);
    }
};
