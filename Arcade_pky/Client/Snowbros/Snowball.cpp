#include "pch.h"

#include "Snowball.h"

#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Snowbros/SnowballComponent.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool Snowball::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto rootComp = CreateSceneComponent<SpriteComponent>("Root");
    rootComp->SetRenderLayer("Snowball");
    rootComp->SetShader("SpriteShader");
    SetRoot(rootComp);

    Ptr<Animation2D> animation = rootComp->CreateAnimation();
    animation->SetAnimationSequence("snowball");
    animation->ChangeAnimationClip("snowball_forming");
    animation->Pause();

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(rootComp);
    collider->SetBoxSize({1.f, 1.f});
    collider->SetCollisionProfile("Snowball");

    auto kinematicComp = CreateActorComponent<PlatformerKinematicComponent>("Kinematic");
    kinematicComp->SetCollider(collider);
    kinematicComp->SetTilemap(tilemap);

    _snowballComponent = CreateActorComponent<SnowballComponent>("Snowball");

    collider->RegisterCollisionCallBack(CollisionState::Enter,
      [=](Weak<CollisionComponent> collider)
      {
          _snowballComponent->CollideWith(collider);
      });

    return true;
}

void Snowball::Destroy()
{
    Actor::Destroy();
}

Ptr<class SnowballComponent> Snowball::GetSnowballComponent() const
{
    return _snowballComponent;
}
