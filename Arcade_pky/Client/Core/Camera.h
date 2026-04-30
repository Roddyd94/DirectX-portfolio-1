#pragma once
#include "Actor.h"

class Camera : public Actor
{
public:
    Camera()           = default;
    ~Camera() override = default;

private:
    Ptr<class CameraComponent> _cameraComp;

public:
    bool Init(int32  id,
      Vector3 position,
      Vector3 scale,
      Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
};
