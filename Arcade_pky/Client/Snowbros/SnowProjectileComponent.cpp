#include "pch.h"

#include "SnowProjectileComponent.h"

#include "Core/Actor.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Animation/SpriteInstanceComponent.h"
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
    if (_setHit)
        _isHit = true;

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
        velocityX = _rangeUp ? velocityXPhase1RangeUp : velocityXPhase1Normal;
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
    _setHit  = true;
    _accTime = 0.f;

    _kinematic->SetVelocity(Vector2::zero);
    auto sprite = GetOwner()->FindSceneComponent<SpriteInstanceComponent>("Root");
    sprite->ChangeAnimation("projectile_hit");
}

bool SnowProjectileComponent::IsHit() const
{
    return _isHit;
}

bool SnowProjectileComponent::IsPoweredUp() const
{
    return _powerUp;
}

int32 SnowProjectileComponent::GetPlayerNumber() const
{
    return _playerNumber;
}

void SnowProjectileComponent::SetPlayer(int32 number)
{
    _playerNumber = number;
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
