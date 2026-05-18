#include "pch.h"

#include "SnowProjectile.h"

#include "SnowProjectileComponent.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool SnowProjectile::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto rootComp = CreateSceneComponent<SpriteComponent>("Root");
    rootComp->SetRenderLayer("PlayerProjectile");
    rootComp->SetShader("SpriteShader");
    SetRoot(rootComp);

    Ptr<Animation2D> animation = rootComp->CreateAnimation();
    animation->SetAnimationSequence("projectile");
    animation->ChangeAnimationClip("projectile");
    animation->AddNotify("projectile_hit", animation->GetClipFrameCount("projectile_hit"),
      [this]()
      {
          SetActive(false);
      });

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(rootComp);
    collider->SetBoxSize({0.6f, 0.6f});
    collider->SetCollisionProfile("PlayerProjectile");

    auto kinematic = CreateActorComponent<PlatformerKinematicComponent>("Kinematic");
    kinematic->SetCollider(collider);
    kinematic->SetTilemap(tilemap);

    _projectileComponent = CreateActorComponent<SnowProjectileComponent>("Projectile");
    _projectileComponent->SetKinematic(kinematic);
    collider->RegisterCollisionCallBack(
      CollisionState::Enter, Raw(_projectileComponent), &SnowProjectileComponent::OnCollisionWith);
    kinematic->RegisterOnCollidedWithBlock(
      Raw(_projectileComponent), &SnowProjectileComponent::OnCollision);
    kinematic->RegisterOnCollidedWithFloor(
      Raw(_projectileComponent), &SnowProjectileComponent::OnCollision);
    kinematic->RegisterOnCollidedWithBoundary(
      Raw(_projectileComponent), &SnowProjectileComponent::OnCollision);

    return true;
}

Ptr<class SnowProjectileComponent> SnowProjectile::GetProjectileComponent() const
{
    return _projectileComponent;
}

bool SnowProjectile::IsPoweredUp() const
{
    return _projectileComponent->_powerUp;
}

void SnowProjectile::SetPowerUp(bool powerUp)
{
    _projectileComponent->_powerUp = powerUp;
    Ptr<Animation2D> animation     = Cast<SceneComponent, SpriteComponent>(_root)->GetAnimation();

    if (powerUp)
        animation->ChangeAnimationClip("projectile_power");
    else
        animation->ChangeAnimationClip("projectile");
}

void SnowProjectile::SetRangeUp(bool rangeUp)
{
    _projectileComponent->_rangeUp = rangeUp;
}

void SnowProjectile::SetDirection(float direction)
{
    _projectileComponent->_direction = direction;
    auto sprite                      = Cast<SceneComponent, SpriteComponent>(_root);
    sprite->SetFlipX(direction > 0);
}
