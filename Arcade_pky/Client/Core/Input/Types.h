#pragma once

namespace MouseButtonType
{
    enum Type
    {
        LButton = 0,
        RButton = 1,
        MButton = 2,
        Wheel   = 2,
        End
    };
}

namespace ButtonEventType
{
    enum Type
    {
        Down,
        Hold,
        Up,
        End
    };
}

enum class InputSystemType
{
    DInput,
    Window,
    End
};
