#pragma once
#include <cmath>

union Vector2
{
    float data[2] = {};
    struct
    {
        float x;
        float y;
    };

public:
    Vector2()                         = default;
    Vector2(const Vector2& other)     = default;
    Vector2(Vector2&& other) noexcept = default;

    Vector2(float x, float y);

    static const Vector2 zero;
    static const Vector2 one;
    static const Vector2 axisX;
    static const Vector2 axisY;

    Vector2& operator=(Vector2 other);

    Vector2& operator+=(Vector2 other);
    Vector2& operator+=(int32 value);
    Vector2& operator+=(float value);
    Vector2& operator+=(double value);

    Vector2& operator-=(Vector2 other);
    Vector2& operator-=(int32 value);
    Vector2& operator-=(float value);
    Vector2& operator-=(double value);

    Vector2& operator*=(int32 value);
    Vector2& operator*=(float value);
    Vector2& operator*=(double value);

    Vector2& operator/=(int32 value);
    Vector2& operator/=(float value);
    Vector2& operator/=(double value);

    Vector2 operator+(Vector2 other) const;
    Vector2 operator-(Vector2 other) const;

    Vector2 operator-() const;

    bool operator==(Vector2 other);
    bool operator!=(Vector2 other);

    void Set(int32 value);
    void Set(float value);
    void Set(double value);

    static Vector2 Normalize(Vector2 v);
    static float   SquareLength(Vector2 v);
    static float   SquareDistance(Vector2 v, Vector2 u);
    static float   Length(Vector2 v);
    static float   Distance(Vector2 v, Vector2 u);
    static float   Dot(Vector2 v, Vector2 u);

    Vector2 Normalize() const;
    float   SquareLength() const;
    float   SquareDistance(Vector2 other) const;
    float   Length() const;
    float   Distance(Vector2 other) const;
    float   Dot(Vector2 other) const;
};

Vector2 operator*(float lhs, Vector2 rhs);
Vector2 operator*(double lhs, Vector2 rhs);
Vector2 operator*(int32 lhs, Vector2 rhs);

Vector2 operator*(Vector2 lhs, float rhs);
Vector2 operator*(Vector2 lhs, double rhs);
Vector2 operator*(Vector2 lhs, int32 rhs);

Vector2 operator/(Vector2 lhs, float rhs);
Vector2 operator/(Vector2 lhs, double rhs);
Vector2 operator/(Vector2 lhs, int32 rhs);
