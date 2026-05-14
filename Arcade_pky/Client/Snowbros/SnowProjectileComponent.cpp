#include "pch.h"

#include "SnowProjectileComponent.h"

#include "Platformer/PlatformerKinematicComponent.h"

bool SnowProjectileComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);
    _velocity.y = initialVelocityY;

    return true;
}

void SnowProjectileComponent::Destroy()
{
    ActorComponent::Destroy();
}

void SnowProjectileComponent::Tick(float deltaTime)
{
    _accTime += deltaTime;
    float velocityX = 0.f;

    if (_accTime < phaseTime)
        velocityX = _rangeUp ? velocityXPhaseRangeUp1 : velocityXPhaseNormal1;
    else
    {
        velocityX = velocityXPhase2;
        velocityX -= (_accTime - phaseTime) * airDrag;
        velocityX = std::max(0.f, velocityX);
    }

    _kinematic->MoveX(_direction * velocityX);
    _kinematic->AddGravity(deltaTime);
}

void SnowProjectileComponent::SetKinematic(Ptr<class PlatformerKinematicComponent> kinematic)
{
    _kinematic = kinematic;
}

void SnowProjectileComponent::SetDirection(float direction)
{
    _direction = direction;
}
