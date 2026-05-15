#include "pch.h"

#include "Rect.h"

Rect::Rect(float left, float top, float right, float bottom)
    : left(left), top(top), right(right), bottom(bottom)
{
}

Rect::Rect(Vector2 leftBottom, Vector2 rightTop) : leftBottom(leftBottom), rightTop(rightTop) {}

void Rect::Move(Vector2 delta)
{
    left += delta.x;
    right += delta.x;
    top += delta.y;
    bottom += delta.y;
}

void Rect::MoveX(float deltaX)
{
    left += deltaX;
    right += deltaX;
}

void Rect::MoveY(float deltaY)
{
    top += deltaY;
    bottom += deltaY;
}
