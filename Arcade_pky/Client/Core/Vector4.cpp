#include "pch.h"

#include "Vector4.h"

const Vector4 Vector4::zero{0.f, 0.f, 0.f, 0.f};
const Vector4 Vector4::one{1.f, 1.f, 1.f, 1.f};
const Vector4 Vector4::axisX{1.f, 0.f, 0.f, 0.f};
const Vector4 Vector4::axisY{0.f, 1.f, 0.f, 0.f};
const Vector4 Vector4::axisZ{0.f, 0.f, 1.f, 0.f};
const Vector4 Vector4::axisW{0.f, 0.f, 0.f, 1.f};

Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vector4& Vector4::operator+=(Vector4 other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;

    return *this;
}

Vector4& Vector4::operator+=(int32 value)
{
    float v = static_cast<float>(value);
    return operator+=(v);
}

Vector4& Vector4::operator+=(float value)
{
    x += value;
    y += value;
    z += value;
    w += value;

    return *this;
}

Vector4& Vector4::operator+=(double value)
{
    float v = static_cast<float>(value);
    return operator+=(v);
}

Vector4& Vector4::operator-=(Vector4 other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;

    return *this;
}

Vector4& Vector4::operator-=(int32 value)
{
    float v = static_cast<float>(value);
    return operator-=(v);
}

Vector4& Vector4::operator-=(float value)
{
    x -= value;
    y -= value;
    z -= value;
    w -= value;

    return *this;
}

Vector4& Vector4::operator-=(double value)
{
    float v = static_cast<float>(value);
    return operator-=(v);
}

Vector4& Vector4::operator*=(int32 value)
{
    float v = static_cast<float>(value);
    return operator*=(v);
}

Vector4& Vector4::operator*=(float value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;

    return *this;
}

Vector4& Vector4::operator*=(double value)
{
    float v = static_cast<float>(value);
    return operator*=(v);
}

Vector4& Vector4::operator/=(int32 value)
{
    float v = static_cast<float>(value);
    return operator/=(v);
}

Vector4& Vector4::operator/=(float value)
{
    x /= value;
    y /= value;
    z /= value;
    w /= value;

    return *this;
}

Vector4& Vector4::operator/=(double value)
{
    float v = static_cast<float>(value);
    return operator/=(v);
}

Vector4 Vector4::operator+(Vector4 other) const
{
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

Vector4 Vector4::operator-(Vector4 other) const
{
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Vector4 Vector4::operator-() const
{
    return {-x, -y, -z, -w};
}

bool Vector4::operator==(Vector4 other)
{
    return fabsf(x - other.x) < FLT_EPSILON && fabsf(y - other.y) < FLT_EPSILON
        && fabsf(z - other.z) < FLT_EPSILON && fabsf(w - other.w) < FLT_EPSILON;
}

bool Vector4::operator!=(Vector4 other)
{
    return !(*this == other);
}

float& Vector4::operator[](size_t index)
{
    return data[index];
}

void Vector4::Set(int32 value)
{
    float v = static_cast<float>(value);
    Set(v);
}

void Vector4::Set(float value)
{
    x = value;
    y = value;
    z = value;
    w = value;
}

void Vector4::Set(double value)
{
    float v = static_cast<float>(value);
    Set(v);
}

Vector4 Vector4::Normalize(Vector4 v)
{
    float length = v.Length();
    return {v.x / length, v.y / length, v.z / length, v.w / length};
}

float Vector4::SquareLength(Vector4 v)
{
    return v.SquareLength();
}

float Vector4::SquareDistance(Vector4 v, Vector4 u)
{
    return v.SquareDistance(u);
}

float Vector4::Length(Vector4 v)
{
    return v.Length();
}

float Vector4::Distance(Vector4 v, Vector4 u)
{
    return v.Distance(u);
}

float Vector4::Dot(Vector4 v, Vector4 u)
{
    return v.Dot(u);
}

Vector4 Vector4::Normalize() const
{
    return Normalize(*this);
}

float Vector4::SquareLength() const
{
    return x * x + y * y + z * z + w * w;
}

float Vector4::SquareDistance(Vector4 other) const
{
    return (*this - other).SquareLength();
}

float Vector4::Length() const
{
    return sqrtf(SquareLength());
}

float Vector4::Distance(Vector4 other) const
{
    return (*this - other).Length();
}

float Vector4::Dot(Vector4 other) const
{
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

Vector4 operator*(float lhs, Vector4 rhs)
{
    return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w};
}

Vector4 operator*(double lhs, Vector4 rhs)
{
    float v = static_cast<float>(lhs);
    return operator*(v, rhs);
}

Vector4 operator*(int32 lhs, Vector4 rhs)
{
    float v = static_cast<float>(lhs);
    return operator*(v, rhs);
}

Vector4 operator*(Vector4 lhs, float rhs)
{
    return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs};
}

Vector4 operator*(Vector4 lhs, double rhs)
{
    float v = static_cast<float>(rhs);
    return operator*(lhs, v);
}

Vector4 operator*(Vector4 lhs, int32 rhs)
{
    float v = static_cast<float>(rhs);
    return operator*(lhs, v);
}

Vector4 operator/(Vector4 lhs, float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs};
}

Vector4 operator/(Vector4 lhs, double rhs)
{
    float v = static_cast<float>(rhs);
    return operator/(lhs, v);
}

Vector4 operator/(Vector4 lhs, int32 rhs)
{
    float v = static_cast<float>(rhs);
    return operator/(lhs, v);
}
