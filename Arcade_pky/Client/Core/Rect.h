#pragma once

union Rect
{
    float data[4] = {};
    struct
    {
        Vector2 start;
        Vector2 end;
    };
    struct
    {
        float left;
        float top;
        float right;
        float bottom;
    };

    Rect()                      = default;
    Rect(const Rect& other)     = default;
    Rect(Rect&& other) noexcept = default;
    Rect(float left, float top, float right, float bottom);
    Rect(Vector2 start, Vector2 end);
};
