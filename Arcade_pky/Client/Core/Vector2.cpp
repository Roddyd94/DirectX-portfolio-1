#include "pch.h"

#include "Vector2.h"

const Vector2 Vector2::zero{0.f, 0.f};
const Vector2 Vector2::one{1.f, 1.f};
const Vector2 Vector2::axisX{1.f, 0.f};
const Vector2 Vector2::axisY{0.f, 1.f};

Vector2::Vector2() : Vector2(0.f, 0.f) {}

Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2& Vector2::operator=(Vector2 other)
{
    x = other.x;
    y = other.y;

    return *this;
}

Vector2& Vector2::operator+=(Vector2 other)
{
    x += other.x;
    y += other.y;

    return *this;
}

Vector2& Vector2::operator+=(int32 value)
{
    float v = static_cast<float>(value);
    return operator+=(v);
}

Vector2& Vector2::operator+=(float value)
{
    x += value;
    y += value;

    return *this;
}

Vector2& Vector2::operator+=(double value)
{
    float v = static_cast<float>(value);
    return operator+=(v);
}

Vector2& Vector2::operator-=(Vector2 other)
{
    x -= other.x;
    y -= other.y;

    return *this;
}

Vector2& Vector2::operator-=(int32 value)
{
    float v = static_cast<float>(value);
    return operator-=(v);
}

Vector2& Vector2::operator-=(float value)
{
    x -= value;
    y -= value;

    return *this;
}

Vector2& Vector2::operator-=(double value)
{
    float v = static_cast<float>(value);
    return operator-=(v);
}

Vector2& Vector2::operator*=(int32 value)
{
    float v = static_cast<float>(value);
    return operator*=(v);
}

Vector2& Vector2::operator*=(float value)
{
    x *= value;
    y *= value;

    return *this;
}

Vector2& Vector2::operator*=(double value)
{
    float v = static_cast<float>(value);
    return operator*=(v);
}

Vector2& Vector2::operator/=(int32 value)
{
    float v = static_cast<float>(value);
    return operator/=(v);
}

Vector2& Vector2::operator/=(float value)
{
    x /= value;
    y /= value;

    return *this;
}

Vector2& Vector2::operator/=(double value)
{
    float v = static_cast<float>(value);
    return operator/=(v);
}

Vector2 Vector2::operator+(Vector2 other) const
{
    return {x + other.x, y + other.y};
}

Vector2 Vector2::operator-(Vector2 other) const
{
    return {x - other.x, y - other.y};
}

Vector2 Vector2::operator-() const
{
    return {-x, -y};
}

bool Vector2::operator==(Vector2 other)
{
    return fabsf(x - other.x) < FLT_EPSILON && fabsf(y - other.y) < FLT_EPSILON;
}

bool Vector2::operator!=(Vector2 other)
{
    return !(*this == other);
}

void Vector2::Set(int32 value)
{
    float v = static_cast<float>(value);
    Set(v);
}

void Vector2::Set(float value)
{
    x = value;
    y = value;
}

void Vector2::Set(double value)
{
    float v = static_cast<float>(value);
    Set(v);
}

Vector2 Vector2::Normalize(Vector2 v)
{
    float length = v.Length();
    return {v.x / length, v.y / length};
}

float Vector2::SquareLength(Vector2 v)
{
    return v.SquareLength();
}

float Vector2::SquareDistance(Vector2 v, Vector2 u)
{
    return v.SquareDistance(u);
}

float Vector2::Length(Vector2 v)
{
    return v.Length();
}

float Vector2::Distance(Vector2 v, Vector2 u)
{
    return v.Distance(u);
}

float Vector2::Dot(Vector2 v, Vector2 u)
{
    return v.Dot(u);
}

Vector2 Vector2::Normalize() const
{
    return Normalize(*this);
}

float Vector2::SquareLength() const
{
    return x * x + y * y;
}

float Vector2::SquareDistance(Vector2 other) const
{
    return (*this - other).SquareLength();
}

float Vector2::Length() const
{
    return sqrtf(SquareLength());
}

float Vector2::Distance(Vector2 other) const
{
    return (*this - other).Length();
}

float Vector2::Dot(Vector2 other) const
{
    return x * other.x + y * other.y;
}

Vector2 operator*(float lhs, Vector2 rhs)
{
    return {lhs * rhs.x, lhs * rhs.y};
}

Vector2 operator*(double lhs, Vector2 rhs)
{
    float v = static_cast<float>(lhs);
    return operator*(v, rhs);
}

Vector2 operator*(int32 lhs, Vector2 rhs)
{
    float v = static_cast<float>(lhs);
    return operator*(v, rhs);
}

Vector2 operator*(Vector2 lhs, float rhs)
{
    return {lhs.x * rhs, lhs.y * rhs};
}

Vector2 operator*(Vector2 lhs, double rhs)
{
    float v = static_cast<float>(rhs);
    return operator*(lhs, v);
}

Vector2 operator*(Vector2 lhs, int32 rhs)
{
    float v = static_cast<float>(rhs);
    return operator*(lhs, v);
}

Vector2 operator/(Vector2 lhs, float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs};
}

Vector2 operator/(Vector2 lhs, double rhs)
{
    float v = static_cast<float>(rhs);
    return operator/(lhs, v);
}

Vector2 operator/(Vector2 lhs, int32 rhs)
{
    float v = static_cast<float>(rhs);
    return operator/(lhs, v);
}
