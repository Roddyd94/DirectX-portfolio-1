#pragma once

namespace AIEventState
{
    enum Type
    {
        Enter,
        Exit,
        Tick,
        End
    };
}

enum class ConditionOperator
{
    And,
    Or,
    End
};
