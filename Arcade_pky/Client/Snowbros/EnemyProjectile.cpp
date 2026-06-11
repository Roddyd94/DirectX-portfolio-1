#include "pch.h"

#include "EnemyProjectile.h"

#include "Core/ResourceManager.h"
#include "Core/TimeManager.h"

#include "IndexedSpriteInstanceComponent.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "AI/AIComponent.h"
#include "AI/AIController.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Palette.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool EnemyProjectile::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto rootComp = CreateSceneComponent<IndexedSpriteInstanceComponent>("Root");
    // rootComp->
    auto palette = FIND_PALETTE("snowball");
    rootComp->SetPaletteNumber(palette->GetID());
    rootComp->SetRenderLayer("EnemyProjectile");
    SetRoot(rootComp);

    Ptr<Animation2D> animation = rootComp->CreateAnimation();
    animation->SetAnimationSequence("spitter_fire");
    animation->ChangeAnimationClip("spitter_fire");
    animation->AddNotify("spitter_fire", animation->GetClipFrameCount("spitter_fire"),
      [this]()
      {
          SetActive(false);
      });

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(rootComp);
    collider->SetBoxSize({1.f, 1.f});
    collider->SetCollisionProfile("EnemyProjectile");

    auto kinematic = CreateActorComponent<PlatformerKinematicComponent>("Kinematic");
    kinematic->SetCollider(collider);

    return true;
}

void EnemyProjectile::Destroy()
{
    Actor::Destroy();
}

void EnemyProjectile::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
    auto kinematic = FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    Vector2 velocity = kinematic->GetVelocity();

    if (kinematic->IsColliderMovingAgainstBoundaryX(velocity.x * deltaTime)
        || kinematic->IsColliderMovingAgainstWallX(velocity.x * deltaTime))
        kinematic->SetVelocityX(0.f);
}

void EnemyProjectile::SetDirection(float direction)
{
    auto sprite = FindSceneComponent<IndexedSpriteInstanceComponent>("Root");
    sprite->SetFlipX(direction > 0 ? true : false);

    auto kinematic = FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    kinematic->SetVelocityX(direction * 5.f);
}
