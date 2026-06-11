#include "pch.h"

#include "AICondition.h"

void AIConditionBase::Destroy() {}

void AIConditionBase::SetName(const std::string& name)
{
    _name = name;
}

void AIConditionBase::SetOperator(ConditionOperator op)
{
    _operator = op;
}

bool AICondition::CheckCondition(float deltaTime) const
{
    if (_conditions.size() < 1)
        return false;

    if (_operator == ConditionOperator::And)
    {
        for (auto& condition : _conditions)
        {
            if (!condition(deltaTime))
                return false;
        }

        return true;
    }
    else
    {
        for (auto& condition : _conditions)
        {
            if (condition(deltaTime))
                return true;
        }

        return false;
    }

    return true;
}

bool AICompositeCondition::CheckCondition(float deltaTime) const
{
    if (_conditions.size() < 1)
        return false;

    if (_operator == ConditionOperator::And)
    {
        for (auto& condition : _conditions)
        {
            if (!condition->CheckCondition(deltaTime))
                return false;
        }

        return true;
    }
    else
    {
        for (auto& condition : _conditions)
        {
            if (condition->CheckCondition(deltaTime))
                return true;
        }

        return false;
    }

    return true;
}

void AICompositeCondition::AddCondition(Ptr<AIConditionBase> condition)
{
    _conditions.push_back(condition);
}
