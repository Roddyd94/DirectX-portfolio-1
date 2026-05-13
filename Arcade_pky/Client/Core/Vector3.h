#pragma once
#include "EngineMath.h"

#include <cmath>

union Matrix;

union Vector3
{
    float data[3] = {0.f, 0.f, 0.f};
    struct
    {
        float x;
        float y;
        float z;
    };
    struct
    {
        float r;
        float g;
        float b;
    };

public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector3& other)     = default;
    Vector3(Vector3&& other) noexcept = default;
    Vector3(const DirectX::XMVECTOR& v);

    operator DirectX::XMVECTOR() const;

    static const Vector3 zero;
    static const Vector3 one;
    static const Vector3 axisX;
    static const Vector3 axisY;
    static const Vector3 axisZ;

    Vector3& operator=(Vector3 other);

    Vector3& operator+=(Vector3 other);
    Vector3& operator+=(int32 value);
    Vector3& operator+=(float value);
    Vector3& operator+=(double value);

    Vector3& operator-=(Vector3 other);
    Vector3& operator-=(int32 value);
    Vector3& operator-=(float value);
    Vector3& operator-=(double value);

    Vector3& operator*=(int32 value);
    Vector3& operator*=(float value);
    Vector3& operator*=(double value);

    Vector3& operator/=(int32 value);
    Vector3& operator/=(float value);
    Vector3& operator/=(double value);

    Vector3 operator+(Vector3 other) const;
    Vector3 operator-(Vector3 other) const;

    Vector3 operator-() const;

    bool operator==(Vector3 other);
    bool operator!=(Vector3 other);

    Vector2 ToVector2() const;

    void Set(int32 value);
    void Set(float value);
    void Set(double value);

    static Vector3 Normalize(Vector3 v);
    static float   SquareLength(Vector3 v);
    static float   SquareDistance(Vector3 v, Vector3 u);
    static float   Length(Vector3 v);
    static float   Distance(Vector3 v, Vector3 u);
    static float   Dot(Vector3 v, Vector3 u);
    static Vector3 Cross(Vector3 v, Vector3 u);
    static Vector3 TransformNormal(Vector3 v, const Matrix& mat);
    static Vector3 TransformCoord(Vector3 v, const Matrix& mat);

    Vector3 Normalize() const;
    float   SquareLength() const;
    float   SquareDistance(Vector3 other) const;
    float   Length() const;
    float   Distance(Vector3 other) const;
    float   Dot(Vector3 other) const;
    Vector3 Cross(Vector3 other) const;
    Vector3 TransformNormal(const Matrix& mat) const;
    Vector3 TransformCoord(const Matrix& mat) const;
};

Vector3 operator*(float lhs, Vector3 rhs);
Vector3 operator*(double lhs, Vector3 rhs);
Vector3 operator*(int32 lhs, Vector3 rhs);

Vector3 operator*(Vector3 lhs, float rhs);
Vector3 operator*(Vector3 lhs, double rhs);
Vector3 operator*(Vector3 lhs, int32 rhs);

Vector3 operator/(Vector3 lhs, float rhs);
Vector3 operator/(Vector3 lhs, double rhs);
Vector3 operator/(Vector3 lhs, int32 rhs);
