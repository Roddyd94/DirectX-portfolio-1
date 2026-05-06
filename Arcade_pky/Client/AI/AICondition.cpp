#include "pch.h"

#include "AICondition.h"

void AIConditionBase::SetTransitionRule(TransitionRule rule)
{
    _rule = rule;
}

bool AICondition::CheckCondition(float deltaTime) const
{
    if (_conditions.size() < 1)
        return false;

    if (_rule == TransitionRule::And)
    {
        for (auto& condition : _conditions)
        {
            if (!condition())
                return false;
        }

        return true;
    }
    else
    {
        for (auto& condition : _conditions)
        {
            if (condition())
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

    if (_rule == TransitionRule::And)
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
