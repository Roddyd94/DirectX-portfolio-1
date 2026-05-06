#pragma once

namespace MouseButtonType
{
    enum Type : unsigned short
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
    enum Type : unsigned short
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
