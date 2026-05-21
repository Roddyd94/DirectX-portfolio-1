#pragma once

union Rect
{
    float data[4] = {};
    struct
    {
        Vector2 leftBottom;
        Vector2 rightTop;
    };
    struct
    {
        float left;
        float bottom;
        float right;
        float top;
    };

    Rect()                                 = default;
    Rect(const Rect& other)                = default;
    Rect(Rect&& other) noexcept            = default;
    Rect& operator=(const Rect& other)     = default;
    Rect& operator=(Rect&& other) noexcept = default;
    Rect(float left, float top, float right, float bottom);
    Rect(Vector2 leftBottom, Vector2 rightTop);

    void Move(Vector2 delta);
    void MoveX(float deltaX);
    void MoveY(float deltaY);

    float   GetCenterX() const;
    float   GetCenterY() const;
    Vector2 GetCenter() const;
};
