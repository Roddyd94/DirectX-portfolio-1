#include "pch.h"

#include "PlatformerMovementComponent.h"

#include "PlatformerKinematicComponent.h"

bool PlatformerMovementComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);

    return true;
}

void PlatformerMovementComponent::Destroy()
{
    ActorComponent::Destroy();
}

void PlatformerMovementComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);
}

float PlatformerMovementComponent::GetDirection() const
{
    return _direction;
}

void PlatformerMovementComponent::SetDirection(float direction)
{
    _direction = direction;
}

void PlatformerMovementComponent::SetKinematic(Ptr<class PlatformerKinematicComponent> kinematic)
{
    _kinematic = kinematic;
}

void PlatformerMovementComponent::SetJumpForce(float force)
{
    _jumpForce = force;
}

void PlatformerMovementComponent::MoveLeft(float speedX)
{
    _kinematic->SetVelocityX(-speedX);
    _direction = -1.f;
}

void PlatformerMovementComponent::MoveRight(float speedX)
{
    _kinematic->SetVelocityX(speedX);
    _direction = 1.f;
}

void PlatformerMovementComponent::Stop()
{
    _kinematic->SetVelocityX(0.f);
}

void PlatformerMovementComponent::Jump()
{
    _kinematic->AddForce({0.f, _jumpForce});
}

void PlatformerMovementComponent::AccelerateGravity(float deltaTime)
{
    _kinematic->AddGravity(deltaTime);
}
