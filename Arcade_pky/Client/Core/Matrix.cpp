#include "pch.h"

#include "Matrix.h"

Matrix::Matrix() : m(DirectX::XMMatrixIdentity()) {}

Matrix::Matrix(const Matrix& m) : m(m.m) {}

Matrix::Matrix(Matrix&& m) noexcept : m(m.m) {}

Matrix::Matrix(const DirectX::XMMATRIX& m) : m(m) {}

Matrix::Matrix(DirectX::XMMATRIX&& m) noexcept : m(m) {}

Matrix::Matrix(const Vector4 v[4])
{
    memcpy(this->v, v, sizeof(Vector4) * 4);
}

const Matrix Matrix::identity = DirectX::XMMatrixIdentity();

Matrix& Matrix::operator=(const Matrix& m)
{
    this->m = m.m;
    return *this;
}

Matrix& Matrix::operator=(const DirectX::XMMATRIX& m)
{
    this->m = m;
    return *this;
}

Matrix& Matrix::operator=(const Vector4 v[4])
{
    memcpy(this->v, v, sizeof(Vector4) * 4);
    return *this;
}

Matrix Matrix::operator*(const Matrix& m) const
{
    return Matrix(this->m * m.m);
}

Matrix Matrix::operator*(const DirectX::XMMATRIX& m) const
{
    return Matrix(this->m * m);
}

Vector4& Matrix::operator[](size_t index)
{
    return v[index];
}

void Matrix::ToIdentity()
{
    m = DirectX::XMMatrixIdentity();
}

void Matrix::Transpose()
{
    m = DirectX::XMMatrixTranspose(m);
}

void Matrix::Inverse()
{
    DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(m);
    m                     = DirectX::XMMatrixInverse(&det, m);
}

void Matrix::Scaling(Vector3 v)
{
    Scaling(v.x, v.y, v.z);
}

void Matrix::Scaling(float x, float y, float z)
{
    m = DirectX::XMMatrixScaling(x, y, z);
}

void Matrix::Scaling(Vector2 v)
{
    Scaling(v.x, v.y);
}

void Matrix::Scaling(float x, float y)
{
    m = DirectX::XMMatrixScaling(x, y, 1.f);
}

void Matrix::Rotation(Vector3 rot)
{
    Rotation(rot.x, rot.y, rot.z);
}

void Matrix::Rotation(float x, float y, float z)
{
    float xRad = DirectX::XMConvertToRadians(x);
    float yRad = DirectX::XMConvertToRadians(y);
    float zRad = DirectX::XMConvertToRadians(z);

    DirectX::XMVECTOR quat
      = DirectX::XMQuaternionRotationRollPitchYaw(xRad, yRad, zRad);
    m = DirectX::XMMatrixRotationQuaternion(quat);
}

void Matrix::RotationX(float x)
{
    float xRad = DirectX::XMConvertToRadians(x);
    m          = DirectX::XMMatrixRotationX(xRad);
}

void Matrix::RotationY(float y)
{
    float yRad = DirectX::XMConvertToRadians(y);
    m          = DirectX::XMMatrixRotationY(yRad);
}

void Matrix::RotationZ(float z)
{
    float zRad = DirectX::XMConvertToRadians(z);
    m          = DirectX::XMMatrixRotationZ(zRad);
}

void Matrix::RotationAxis(Vector3 axis, float deg)
{
    float             rad    = DirectX::XMConvertToRadians(deg);
    DirectX::XMVECTOR xmAxis = DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)&axis);

    m = DirectX::XMMatrixRotationAxis(xmAxis, rad);
}

void Matrix::Translation(Vector3 v)
{
    Translation(v.x, v.y, v.z);
}

void Matrix::Translation(float x, float y, float z)
{
    m = DirectX::XMMatrixTranslation(x, y, z);
}

void Matrix::Translation(Vector2 v)
{
    Translation(v.x, v.y, 0);
}

void Matrix::Translation(float x, float y)
{
    Translation(x, y, 0);
}

void Matrix::ExtractScale(OUT Vector3& outScale) const
{
    outScale = ExtractScale();
}

void Matrix::ExtractPosition(OUT Vector3& outPosition) const
{
    outPosition = ExtractPosition();
}

void Matrix::ExtractEuler(OUT Vector3& outRotation) const
{
    outRotation = ExtractEuler();
}

Vector3 Matrix::ExtractScale() const
{
    Vector3 result;
    result.x = sqrtf(_11 * _11 + _12 * _12 + _13 * _13);
    result.y = sqrtf(_21 * _21 + _22 * _22 + _23 * _23);
    result.z = sqrtf(_31 * _31 + _32 * _32 + _33 * _33);

    return result;
}

Vector3 Matrix::ExtractPosition() const
{
    Vector3 result;
    result.x = _41;
    result.y = _42;
    result.z = _43;

    return result;
}

Vector3 Matrix::ExtractEuler() const
{
    Vector3 scale = ExtractScale();

    float r13 = _13 / scale.x;
    float r21 = _21 / scale.y;
    float r22 = _22 / scale.y;
    float r23 = _23 / scale.y;
    float r33 = _33 / scale.z;

    Vector3 result;

    if (fabsf(r23) < 0.999f)
    {
        result.x = DirectX::XMConvertToDegrees(asinf(-r23));
        result.y = DirectX::XMConvertToDegrees(atan2f(r13, r33));
        result.z = DirectX::XMConvertToDegrees(atan2f(r21, r22));
    }
    else
    {
        float r11 = _11 / scale.x;
        float r12 = _12 / scale.x;
        result.x  = DirectX::XMConvertToDegrees(asinf(-r23));
        result.y  = DirectX::XMConvertToDegrees(atan2f(-r12, r11));
        result.z  = 0.f;
    }

    return result;
}
