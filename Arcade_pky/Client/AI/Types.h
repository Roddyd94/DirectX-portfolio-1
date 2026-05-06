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

enum class TransitionRule
{
    And,
    Or,
    End
};
