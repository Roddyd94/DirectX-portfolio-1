#include "pch.h"

#include "ShootComponent.h"

#include "SnowProjectile.h"
#include "Core/Actor.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Input/InputAction.h"
#include "Core/Level.h"
#include "Platformer/PlatformerMovementComponent.h"

bool ShootComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    return ActorComponent::Init(componentID, name, owner);
}

void ShootComponent::Destroy()
{
    ActorComponent::Destroy();
}

void ShootComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);
    _accTime += deltaTime;
}

void ShootComponent::HandleInput(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    if (buttonEvent == ButtonEventType::Down)
    {
        Ptr<Actor>           Actor  = GetOwner();
        Ptr<SpriteComponent> sprite = Actor->FindSceneComponent<SpriteComponent>("Root");

        if (action->GetName() == "ShootGround")
        {
            sprite->ChangeAnimation("player_shoot");
            Shoot();
        }
        else if (action->GetName() == "ShootMidair")
        {
            sprite->ChangeAnimation("player_shoot_midair");
            Shoot();
        }
    }
}

void ShootComponent::Shoot()
{
    if (_accTime < _fireRate)
        return;

    Ptr<Level> level    = GetLevel();
    Vector3    position = GetOwner()->GetWorldPosition();

    Ptr<PlatformerMovementComponent> movement
      = GetOwner()->FindActorComponent<PlatformerMovementComponent>("Movement");
    float direction = movement->GetDirection();

    position.x += _extentX * direction;
    Ptr<SnowProjectile> projectile
      = level->SpawnActor<SnowProjectile>(position, Vector3::one, Vector3::zero);
    projectile->SetPowerUp(_powerUp);
    projectile->SetRangeUp(_rangeUp);
    projectile->SetDirection(direction);

    _accTime = 0.f;
}

void ShootComponent::PowerUp()
{
    _powerUp = true;
}

void ShootComponent::RangeUp()
{
    _rangeUp = true;
}

void ShootComponent::Reset()
{
    _accTime = 0.15f;
    _powerUp = false;
    _rangeUp = false;
}

void ShootComponent::SetExtentX(float extent)
{
    _extentX = extent;
}

void ShootComponent::SetFireRate(float fireRate)
{
    _fireRate = fireRate;
}
