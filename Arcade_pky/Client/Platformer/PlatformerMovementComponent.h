#pragma once
#include "Core/ActorComponent.h"

class PlatformerMovementComponent : public ActorComponent
{
public:
    PlatformerMovementComponent()           = default;
    ~PlatformerMovementComponent() override = default;

public:
    Ptr<class PlatformerKinematicComponent> _kinematic;

    float _direction = -1.f;
    float _speedX    = 2.f;
    float _jumpForce = 6.625f;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    float GetDirection() const;

    void SetKinematic(Ptr<class PlatformerKinematicComponent> kinematic);
    void SetSpeedX(float speed);
    void SetJumpForce(float force);

    void MoveLeft(float speedMultiplier = 1.f);
    void MoveRight(float speedMultiplier = 1.f);
    void Stop();
    void Jump();
    void AccelerateGravity(float deltaTime);
};
