#pragma once

union alignas(16) Matrix
{
    DirectX::XMMATRIX m;
    struct
    {
        float _11, _12, _13, _14;
        float _21, _22, _23, _24;
        float _31, _32, _33, _34;
        float _41, _42, _43, _44;
    };
    Vector4 v[4] = {};

    Matrix();
    Matrix(const Matrix& m);
    Matrix(Matrix&& m) noexcept;
    Matrix(const DirectX::XMMATRIX& m);
    Matrix(DirectX::XMMATRIX&& m) noexcept;
    Matrix(const Vector4 v[4]);

    static const Matrix identity;

    Matrix& operator=(const Matrix& m);
    Matrix& operator=(const DirectX::XMMATRIX& m);
    Matrix& operator=(const Vector4 v[4]);

    Matrix operator*(const Matrix& m) const;
    Matrix operator*(const DirectX::XMMATRIX& m) const;

    Vector4& operator[](size_t index);

    void ToIdentity();
    void Transpose();
    void Inverse();

    void Scaling(Vector3 v);
    void Scaling(float x, float y, float z);
    void Scaling(Vector2 v);
    void Scaling(float x, float y);

    void Rotation(Vector3 v);
    void Rotation(float x, float y, float z);
    void RotationX(float x);
    void RotationY(float y);
    void RotationZ(float z);
    void RotationAxis(Vector3 axis, float angle);

    void Translation(Vector3 v);
    void Translation(float x, float y, float z);
    void Translation(Vector2 v);
    void Translation(float x, float y);

    void ExtractScale(OUT Vector3& outVal) const;
    void ExtractPosition(OUT Vector3& outVal) const;
    void ExtractEuler(OUT Vector3& outVal) const;

    Vector3 ExtractScale() const;
    Vector3 ExtractPosition() const;
    Vector3 ExtractEuler() const;
};
