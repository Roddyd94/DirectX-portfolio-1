#pragma once
#include "EngineMath.h"

#include <cmath>

union Vector4
{
    float data[4] = {0.f, 0.f, 0.f, 0.f};
    struct
    {
        float x;
        float y;
        float z;
        float w;
    };
    struct
    {
        float r;
        float g;
        float b;
        float a;
    };

public:
    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(const Vector4& other)     = default;
    Vector4(Vector4&& other) noexcept = default;

    static const Vector4 zero;
    static const Vector4 one;
    static const Vector4 axisX;
    static const Vector4 axisY;
    static const Vector4 axisZ;
    static const Vector4 axisW;

    Vector4& operator=(Vector4 other);

    Vector4& operator+=(Vector4 other);
    Vector4& operator+=(int32 value);
    Vector4& operator+=(float value);
    Vector4& operator+=(double value);

    Vector4& operator-=(Vector4 other);
    Vector4& operator-=(int32 value);
    Vector4& operator-=(float value);
    Vector4& operator-=(double value);

    Vector4& operator*=(int32 value);
    Vector4& operator*=(float value);
    Vector4& operator*=(double value);

    Vector4& operator/=(int32 value);
    Vector4& operator/=(float value);
    Vector4& operator/=(double value);

    Vector4 operator+(Vector4 other) const;
    Vector4 operator-(Vector4 other) const;

    Vector4 operator-() const;

    bool operator==(Vector4 other);
    bool operator!=(Vector4 other);

    float& operator[](size_t index);

    void Set(int32 value);
    void Set(float value);
    void Set(double value);

    static Vector4 Normalize(Vector4 v);
    static float   SquareLength(Vector4 v);
    static float   SquareDistance(Vector4 v, Vector4 u);
    static float   Length(Vector4 v);
    static float   Distance(Vector4 v, Vector4 u);
    static float   Dot(Vector4 v, Vector4 u);

    Vector4 Normalize() const;
    float   SquareLength() const;
    float   SquareDistance(Vector4 other) const;
    float   Length() const;
    float   Distance(Vector4 other) const;
    float   Dot(Vector4 other) const;
};

Vector4 operator*(float lhs, Vector4 rhs);
Vector4 operator*(double lhs, Vector4 rhs);
Vector4 operator*(int32 lhs, Vector4 rhs);

Vector4 operator*(Vector4 lhs, float rhs);
Vector4 operator*(Vector4 lhs, double rhs);
Vector4 operator*(Vector4 lhs, int32 rhs);

Vector4 operator/(Vector4 lhs, float rhs);
Vector4 operator/(Vector4 lhs, double rhs);
Vector4 operator/(Vector4 lhs, int32 rhs);
