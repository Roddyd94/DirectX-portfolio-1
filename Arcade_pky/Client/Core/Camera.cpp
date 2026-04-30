#include "pch.h"

#include "Camera.h"

#include "CameraComponent.h"

bool Camera::Init(int32 id,
  Vector3        position,
  Vector3        scale,
  Vector3        rotation)
{
    Actor::Init(id, position, scale, rotation);

    Ptr<CameraComponent> camera = CreateSceneComponent<CameraComponent>("Camera");
    SetRoot(camera);

    return true;
}

void Camera::Destroy() {}

void Camera::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}
