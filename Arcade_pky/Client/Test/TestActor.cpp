#include "pch.h"

#include "TestActor.h"

#include "Core/ResourceManager.h"

#include "Core/Animation/SpriteComponent.h"

#ifdef _HAS_COLLISION_MODULE
#include "Core/Collision/AABBCollisionComponent.h"
#endif // _HAS_COLLISION_MODULE

bool TestActor::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Actor::Init(id, position, scale, rotation);

    auto rootComp = CreateSceneComponent<SpriteComponent>("Root");
    rootComp->SetRenderLayer("Player");
    rootComp->SetShader("SpriteShader");

    Ptr<Animation2D> animation = rootComp->CreateAnimation();
    animation->SetAnimationSequence("snowbros_player");
    animation->ChangeAnimationClip("player_walk");

    SetRoot(rootComp);

#ifdef _HAS_COLLISION_MODULE
    auto collisionComp = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collisionComp->AttachToComponent(rootComp);
#endif // _HAS_COLLISION_MODULE

    return true;
}
