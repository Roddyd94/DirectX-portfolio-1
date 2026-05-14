#include "pch.h"

#include "Vector3.h"

#include "Matrix.h"

const Vector3 Vector3::zero{0.f, 0.f, 0.f};
const Vector3 Vector3::one{1.f, 1.f, 1.f};
const Vector3 Vector3::axisX{1.f, 0.f, 0.f};
const Vector3 Vector3::axisY{0.f, 1.f, 0.f};
const Vector3 Vector3::axisZ{0.f, 0.f, 1.f};

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3::Vector3(const DirectX::XMVECTOR& v)
{
    DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, v);
}

Vector3& Vector3::operator=(Vector3 other)
{
    x = other.x;
    y = other.y;
    z = other.z;

    return *this;
}

Vector3::operator DirectX::XMVECTOR() const
{
    return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
}

Vector3& Vector3::operator+=(Vector3 other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

Vector3& Vector3::operator+=(int32 value)
{
    float v = static_cast<float>(value);
    return operator+=(v);
}

Vector3& Vector3::operator+=(float value)
{
    x += value;
    y += value;
    z += value;

    return *this;
}

Vector3& Vector3::operator+=(double value)
{
    float v = static_cast<float>(value);
    return operator+=(v);
}

Vector3& Vector3::operator-=(Vector3 other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;

    return *this;
}

Vector3& Vector3::operator-=(int32 value)
{
    float v = static_cast<float>(value);
    return operator-=(v);
}

Vector3& Vector3::operator-=(float value)
{
    x -= value;
    y -= value;
    z -= value;

    return *this;
}

Vector3& Vector3::operator-=(double value)
{
    float v = static_cast<float>(value);
    return operator-=(v);
}

Vector3& Vector3::operator*=(int32 value)
{
    float v = static_cast<float>(value);
    return operator*=(v);
}

Vector3& Vector3::operator*=(float value)
{
    x *= value;
    y *= value;
    z *= value;

    return *this;
}

Vector3& Vector3::operator*=(double value)
{
    float v = static_cast<float>(value);
    return operator*=(v);
}

Vector3& Vector3::operator/=(int32 value)
{
    float v = static_cast<float>(value);
    return operator/=(v);
}

Vector3& Vector3::operator/=(float value)
{
    x /= value;
    y /= value;
    z /= value;

    return *this;
}

Vector3& Vector3::operator/=(double value)
{
    float v = static_cast<float>(value);
    return operator/=(v);
}

Vector3 Vector3::operator+(Vector3 other) const
{
    return {x + other.x, y + other.y, z + other.z};
}

Vector3 Vector3::operator-(Vector3 other) const
{
    return {x - other.x, y - other.y, z - other.z};
}

Vector3 Vector3::operator-() const
{
    return {-x, -y, -z};
}

bool Vector3::operator==(Vector3 other)
{
    return fabsf(x - other.x) < FLT_EPSILON && fabsf(y - other.y) < FLT_EPSILON
        && fabsf(z - other.z) < FLT_EPSILON;
}

bool Vector3::operator!=(Vector3 other)
{
    return !(*this == other);
}

Vector2 Vector3::ToVector2() const
{
    return {x, y};
}

void Vector3::Set(int32 value)
{
    float v = static_cast<float>(value);
    Set(v);
}

void Vector3::Set(float value)
{
    x = value;
    y = value;
    z = value;
}

void Vector3::Set(double value)
{
    float v = static_cast<float>(value);
    Set(v);
}

Vector3 Vector3::Normalize(Vector3 v)
{
    float length = v.Length();
    return {v.x / length, v.y / length, v.z / length};
}

float Vector3::SquareLength(Vector3 v)
{
    return v.SquareLength();
}

float Vector3::SquareDistance(Vector3 v, Vector3 u)
{
    return v.SquareDistance(u);
}

float Vector3::Length(Vector3 v)
{
    return v.Length();
}

float Vector3::Distance(Vector3 v, Vector3 u)
{
    return v.Distance(u);
}

float Vector3::Dot(Vector3 v, Vector3 u)
{
    return v.Dot(u);
}

Vector3 Vector3::Cross(Vector3 v, Vector3 u)
{
    return v.Cross(u);
}

Vector3 Vector3::TransformNormal(Vector3 v, const Matrix& mat)
{
    return v.TransformNormal(mat);
}

Vector3 Vector3::TransformCoord(Vector3 v, const Matrix& mat)
{
    return v.TransformCoord(mat);
}

Vector3 Vector3::Normalize() const
{
    return Normalize(*this);
}

float Vector3::SquareLength() const
{
    return x * x + y * y + z * z;
}

float Vector3::SquareDistance(Vector3 other) const
{
    return (*this - other).SquareLength();
}

float Vector3::Length() const
{
    return sqrtf(SquareLength());
}

float Vector3::Distance(Vector3 other) const
{
    return (*this - other).Length();
}

float Vector3::Dot(Vector3 other) const
{
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::Cross(Vector3 other) const
{
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

Vector3 Vector3::TransformNormal(const Matrix& mat) const
{
    DirectX::XMMATRIX invTranspose
      = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, mat.m));
    DirectX::XMVECTOR result = DirectX::XMVector3TransformNormal(*this, invTranspose);

    return result;
}

Vector3 Vector3::TransformCoord(const Matrix& mat) const
{
    DirectX::XMVECTOR result = DirectX::XMVector3TransformCoord(*this, mat.m);

    return result;
}

Vector3 operator*(float lhs, Vector3 rhs)
{
    return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
}

Vector3 operator*(double lhs, Vector3 rhs)
{
    float v = static_cast<float>(lhs);
    return operator*(v, rhs);
}

Vector3 operator*(int32 lhs, Vector3 rhs)
{
    float v = static_cast<float>(lhs);
    return operator*(v, rhs);
}

Vector3 operator*(Vector3 lhs, float rhs)
{
    return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}

Vector3 operator*(Vector3 lhs, double rhs)
{
    float v = static_cast<float>(rhs);
    return operator*(lhs, v);
}

Vector3 operator*(Vector3 lhs, int32 rhs)
{
    float v = static_cast<float>(rhs);
    return operator*(lhs, v);
}

Vector3 operator/(Vector3 lhs, float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

Vector3 operator/(Vector3 lhs, double rhs)
{
    float v = static_cast<float>(rhs);
    return operator/(lhs, v);
}

Vector3 operator/(Vector3 lhs, int32 rhs)
{
    float v = static_cast<float>(rhs);
    return operator/(lhs, v);
}
