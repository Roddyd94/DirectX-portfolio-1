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

void PlatformerMovementComponent::SetKinematic(Ptr<class PlatformerKinematicComponent> kinematic)
{
    _kinematic = kinematic;
}

void PlatformerMovementComponent::SetSpeedX(float speed)
{
    _speedX = speed;
}

void PlatformerMovementComponent::SetJumpForce(float force)
{
    _jumpForce = force;
}

void PlatformerMovementComponent::MoveLeft()
{
    _kinematic->MoveX(-_speedX);
}

void PlatformerMovementComponent::MoveRight()
{
    _kinematic->MoveX(_speedX);
}

void PlatformerMovementComponent::Stop()
{
    _kinematic->MoveX(0.f);
}

void PlatformerMovementComponent::Jump()
{
    _kinematic->AddForce({0.f, _jumpForce});
}

void PlatformerMovementComponent::AccelerateGravity(float deltaTime)
{
    _kinematic->AddGravity(deltaTime);
}
