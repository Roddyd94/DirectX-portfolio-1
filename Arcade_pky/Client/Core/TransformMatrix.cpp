#include "pch.h"

#include "TransformMatrix.h"

#include "Transform.h"

void TransformMatrix::SetSRT(const Transform& transform)
{
    scale.Scaling(transform.scale);
    rotation.Rotation(transform.rotation);
    translation.Translation(transform.position);
}

Transform TransformMatrix::CalculateWorld(const Matrix& parentWorld)
{
    Transform transform;

    Matrix matLocal = scale * rotation * translation;
    world = matLocal * parentWorld;

    transform.position = world.ExtractPosition();
    transform.rotation = world.ExtractEuler();
    transform.scale    = world.ExtractScale();

    return transform;
}
