#include "pch.h"

#include "SnowbrosPlayer.h"

#include "Core/Animation/SpriteComponent.h"
#include "Core/Input/InputComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/PlayerComponent.h"
#include "Player/PlayerController.h"
#include "Snowbros/PlayerStateGround.h"
#include "Snowbros/PlayerStateMidair.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

#ifdef _HAS_COLLISION_MODULE
#include "Core/Collision/AABBCollisionComponent.h"
#endif // _HAS_COLLISION_MODULE

bool SnowbrosPlayer::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Player::Init(id, position, scale, rotation);

    Ptr<TilemapLevel> level = Cast<Level, TilemapLevel>(GetLevel());

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

    auto inputComp = _playerController->GetInputComponent();
    inputComp->BindAction(
      "Ground", "MoveLeft", 'A', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Ground", "MoveRight", 'D', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Ground", "Jump", VK_SPACE, Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Midair", "MoveLeft", 'A', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Midair", "MoveRight", 'D', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Snowball", "Jump", VK_SPACE, Raw(_playerComponent), &PlayerComponent::HandleInput);

    auto kinematicComp = CreateActorComponent<PlatformerKinematicComponent>("PlatformerKinematic");
    kinematicComp->SetCollider(collider);
    kinematicComp->SetTilemap(tilemap);
    kinematicComp->RegisterOnStateChangedCallback(PlatformerKinematicState::OnGround,
      [this]()
      {
          _playerComponent->Transition(PlayerStateGround::instance);
      });
    kinematicComp->RegisterOnStateChangedCallback(PlatformerKinematicState::OnAir,
      [this]()
      {
          _playerComponent->Transition(New<PlayerStateMidair>(false));
      });

    auto movementComp = CreateActorComponent<PlatformerMovementComponent>("PlatformerMovement");
    movementComp->SetKinematic(kinematicComp);

    _playerComponent->Transition(PlayerStateGround::instance);

    return true;
}
