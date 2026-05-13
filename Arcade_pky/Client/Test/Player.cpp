#include "pch.h"

#include "Player.h"

#include "Core/ResourceManager.h"

#include "PlayerComponent.h"
#include "TestLevel.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Input/InputComponent.h"
#include "Core/Input/PlayerController.h"
#include "Core/Level.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Tilemap/Tilemap.h"

#ifdef _HAS_COLLISION_MODULE
#include "Core/Collision/AABBCollisionComponent.h"
#endif // _HAS_COLLISION_MODULE

bool Player::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Pawn::Init(id, position, scale, rotation);

    Ptr<TestLevel> level = Cast<Level, TestLevel>(GetLevel());

    Ptr<PlayerController> controller
      = level->SpawnActor<PlayerController>(Vector3::zero, Vector3::zero, Vector3::zero);
    controller->SetName("Controller");
    controller->SetPawn(This<Pawn>());

    Ptr<Tilemap> tilemap = level->GetTilemap();

    auto rootComp = CreateSceneComponent<SpriteComponent>("Root");
    rootComp->SetRenderLayer("Player");
    rootComp->SetShader("SpriteShader");

    Ptr<Animation2D> animation = rootComp->CreateAnimation();
    animation->SetAnimationSequence("snowbros_player");
    animation->ChangeAnimationClip("player_stand");

    SetRoot(rootComp);

#ifdef _HAS_COLLISION_MODULE
    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(rootComp);
    collider->SetBoxSize({1.f, 1.25f});
#endif // _HAS_COLLISION_MODULE

    auto inputComp = controller->GetInputComponent();

    auto playerComp = CreateActorComponent<PlayerComponent>("Player");

    auto movementComp = CreateActorComponent<PlatformerMovementComponent>("PlatformerMovement");
    inputComp->BindAction(
      "Ground", "MoveLeft", 'A', Raw(playerComp), &PlayerComponent::HandleInput);

    inputComp->BindAction(
      "Ground", "MoveRight", 'D', Raw(playerComp), &PlayerComponent::HandleInput);

    inputComp->BindAction(
      "Ground", "Jump", VK_SPACE, Raw(playerComp), &PlayerComponent::HandleInput);

    inputComp->BindAction(
      "Midair", "MoveLeft", 'A', Raw(playerComp), &PlayerComponent::HandleInput);

    inputComp->BindAction(
      "Midair", "MoveRight", 'D', Raw(playerComp), &PlayerComponent::HandleInput);

    inputComp->BindAction(
      "Snowball", "Jump", VK_SPACE, Raw(playerComp), &PlayerComponent::HandleInput);

    auto kinematicComp = CreateActorComponent<PlatformerKinematicComponent>("PlatformerKinematic");
    kinematicComp->SetCollider(collider);
    kinematicComp->SetTilemap(tilemap);

    movementComp->SetKinematic(kinematicComp);

    return true;
}
