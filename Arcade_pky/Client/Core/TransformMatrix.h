#pragma once

struct TransformMatrix
{
    Matrix scale;
    Matrix rotation;
    Matrix translation;
    Matrix world;

    void SetSRT(const struct Transform& transform);
    Transform CalculateWorld(const Matrix& parentWorld = Matrix::identity);
};
