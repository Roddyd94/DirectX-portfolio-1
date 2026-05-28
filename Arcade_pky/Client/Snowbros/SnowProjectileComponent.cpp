#include "pch.h"

#include "SnowProjectileComponent.h"

#include "Core/Actor.h"
#include "Core/Animation/SpriteComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"

bool SnowProjectileComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(componentID, name, owner);

    return true;
}

void SnowProjectileComponent::Destroy()
{
    ActorComponent::Destroy();
}

void SnowProjectileComponent::Tick(float deltaTime)
{
    if (_isHit)
        return;

    _accTime += deltaTime;
    if (_accTime > lifeTime)
    {
        OnCollision();
        return;
    }

    float velocityX = 0.f;
    if (_accTime < phaseTime)
        velocityX = _rangeUp ? velocityXPhaseRangeUp1 : velocityXPhaseNormal1;
    else
    {
        velocityX = velocityXPhase2;
        velocityX -= (_accTime - phaseTime) * airDrag;
        velocityX = std::max(0.f, velocityX);
    }

    _kinematic->SetVelocityX(_direction * velocityX);
    _kinematic->AddGravity(gravityMultiplier * deltaTime);
}

void SnowProjectileComponent::OnCollisionWith(Weak<class CollisionComponent> other)
{
    OnCollision();
}

void SnowProjectileComponent::OnCollision()
{
    _isHit   = true;
    _accTime = 0.f;

    _kinematic->SetVelocity(Vector2::zero);
    Ptr<SpriteComponent> sprite = GetOwner()->FindSceneComponent<SpriteComponent>("Root");
    sprite->ChangeAnimation("projectile_hit");
}

bool SnowProjectileComponent::IsHit() const
{
    return _isHit;
}

void SnowProjectileComponent::SetKinematic(Ptr<class PlatformerKinematicComponent> kinematic)
{
    _kinematic = kinematic;
    _kinematic->SetVelocity({0.f, initialVelocityY});
}

void SnowProjectileComponent::SetDirection(float direction)
{
    _direction = direction;
}
