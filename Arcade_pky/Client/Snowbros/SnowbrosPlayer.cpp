#include "pch.h"

#include "SnowbrosPlayer.h"

#include "PlayerStateGround.h"
#include "PlayerStateMidair.h"
#include "ShootComponent.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Input/InputComponent.h"
#include "Platformer/PlatformerKinematicPlayerComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/PlayerComponent.h"
#include "Player/PlayerController.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool SnowbrosPlayer::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Player::Init(id, position, scale, rotation);
    _playerComponent->SetBlackboard(New<SnowbrosPlayerBlackboard>());

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto rootComp = CreateSceneComponent<SpriteComponent>("Root");
    rootComp->SetRenderLayer("Player");
    rootComp->SetShader("SpriteShader");
    SetRoot(rootComp);

    Ptr<Animation2D> animation = rootComp->CreateAnimation();
    animation->SetAnimationSequence("player");
    animation->ChangeAnimationClip("player_stand");
    animation->AddNotify("player_jump", animation->GetClipFrameCount("player_jump"),
      [animation]()
      {
          animation->ChangeAnimationClip("player_tumble");
      });
    animation->AddNotify("player_tumble", animation->GetClipFrameCount("player_tumble"),
      [animation]()
      {
          animation->ChangeAnimationClip("player_midair");
      });
    animation->AddNotify("player_shoot", animation->GetClipFrameCount("player_shoot"),
      [animation]()
      {
          animation->ChangeAnimationClip("player_stand");
      });
    animation->AddNotify("player_shoot_midair", animation->GetClipFrameCount("player_shoot_midair"),
      [animation]()
      {
          animation->ChangeAnimationClip("player_midair");
      });

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(rootComp);
    collider->SetBoxSize({1.f, 1.f});
    collider->SetCollisionProfile("Player");

    auto shootComponent = CreateActorComponent<ShootComponent>("Shoot");
    shootComponent->SetExtentX(1.f);

    auto inputComp = _playerController->GetInputComponent();
    inputComp->BindAction(
      "Ground", "MoveLeft", 'A', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Ground", "MoveRight", 'D', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Ground", "Jump", 'T', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Ground", "ShootGround", 'R', Raw(shootComponent), &ShootComponent::HandleInput);
    inputComp->BindAction(
      "Midair", "MoveLeft", 'A', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Midair", "MoveRight", 'D', Raw(_playerComponent), &PlayerComponent::HandleInput);
    inputComp->BindAction(
      "Midair", "ShootMidair", 'R', Raw(shootComponent), &ShootComponent::HandleInput);
    inputComp->BindAction(
      "Snowball", "Jump", 'T', Raw(_playerComponent), &PlayerComponent::HandleInput);

    auto kinematicComp = CreateActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");
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
