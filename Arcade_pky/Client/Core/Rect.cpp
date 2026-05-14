#include "pch.h"

#include "Rect.h"

Rect::Rect(float left, float top, float right, float bottom)
    : left(left), top(top), right(right), bottom(bottom)
{
}

Rect::Rect(Vector2 start, Vector2 end) : start(start), end(end) {}
