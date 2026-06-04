#include "pch.h"

#include "SnowbrosPlayer.h"

#include "Core/Collision/CollisionManager.h"

#include "PlayerStateGround.h"
#include "PlayerStateMidair.h"
#include "ShootComponent.h"
#include "SnowballMorphableEnemyStateMachine.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Animation/SpriteInstanceComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Collision/PointCollisionComponent.h"
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

    auto blackboard = New<SnowbrosPlayerBlackboard>();
    _playerComponent->SetBlackboard(blackboard);

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto rootComp = CreateSceneComponent<SpriteInstanceComponent>("Root");
    rootComp->SetRenderLayer("Player");
    // rootComp->SetShader("SpriteShader");
    SetRoot(rootComp);

    Ptr<Animation2D> animation = rootComp->CreateAnimation();
    animation->SetAnimationSequence("player");
    animation->ChangeAnimationClip("player_stand");
    animation->AddNotify("player_jump", animation->GetClipFrameCount("player_jump"),
      [animation = Weak(animation)]()
      {
          Lock(animation)->ChangeAnimationClip("player_tumble");
      });
    animation->AddNotify("player_tumble", animation->GetClipFrameCount("player_tumble"),
      [animation = Weak(animation)]()
      {
          Lock(animation)->ChangeAnimationClip("player_midair");
      });
    animation->AddNotify("player_shoot", animation->GetClipFrameCount("player_shoot"),
      [blackboard = Weak(blackboard), animation = Weak(animation)]()
      {
          if (Lock(blackboard)->speedUpgraded)
              Lock(animation)->ChangeAnimationClip("player_run");
          else
              Lock(animation)->ChangeAnimationClip("player_stand");
      });
    animation->AddNotify("player_shoot_midair", animation->GetClipFrameCount("player_shoot_midair"),
      [animation = Weak(animation)]()
      {
          Lock(animation)->ChangeAnimationClip("player_midair");
      });
    animation->AddNotify("player_kick", animation->GetClipFrameCount("player_kick"),
      [blackboard = Weak(blackboard), animation = Weak(animation),
        playerComponent = Weak(_playerComponent)]()
      {
          if (SnowbrosPlayerStateType::Midair == Lock(playerComponent)->GetStateType())
              Lock(animation)->ChangeAnimationClip("player_midair");
          else if (Lock(blackboard)->speedUpgraded)
              Lock(animation)->ChangeAnimationClip("player_run");
          else
              Lock(animation)->ChangeAnimationClip("player_stand");
      });
    animation->AddNotify("player_kick_midair", animation->GetClipFrameCount("player_kick_midair"),
      [animation = Weak(animation)]()
      {
          Lock(animation)->ChangeAnimationClip("player_midair");
      });

    _headCollider = CreateSceneComponent<PointCollisionComponent>("HeadCollider");
    _headCollider->AttachToComponent(rootComp);
    _headCollider->SetCollisionProfile("PlayerHead");
    _headCollider->SetRelativePosition({0.f, 0.25f});

    _handColliderLeft = CreateSceneComponent<PointCollisionComponent>("HandColliderLeft");
    _handColliderLeft->AttachToComponent(rootComp);
    _handColliderLeft->SetCollisionProfile("PlayerHandLeft");
    _handColliderLeft->SetRelativePosition({-0.26f, -0.1875f});

    _handColliderRight = CreateSceneComponent<PointCollisionComponent>("HandColliderRight");
    _handColliderRight->AttachToComponent(rootComp);
    _handColliderRight->SetCollisionProfile("PlayerHandRight");
    _handColliderRight->SetRelativePosition({0.26f, -0.1875f});

    _footCollider = CreateSceneComponent<PointCollisionComponent>("FootCollider");
    _footCollider->AttachToComponent(rootComp);
    _footCollider->SetCollisionProfile("PlayerFoot");
    _footCollider->SetRelativePosition({0.f, -0.26f});

    _collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    _collider->AttachToComponent(rootComp);
    _collider->SetBoxSize({1.f, 0.9f});
    _collider->SetCollisionProfile("Player");
    _collider->RegisterCollisionCallBack(CollisionState::Enter,
      [playerComponent = Weak(_playerComponent)](Weak<CollisionComponent> collider)
      {
          Lock(playerComponent)->CollideWith(CollisionState::Enter, collider);
      });
    _collider->RegisterCollisionCallBack(CollisionState::Stay,
      [playerComponent = Weak(_playerComponent)](Weak<CollisionComponent> collider)
      {
          Lock(playerComponent)->CollideWith(CollisionState::Stay, collider);
      });
    _collider->RegisterCollisionCallBack(CollisionState::Exit,
      [playerComponent = Weak(_playerComponent)](Weak<CollisionComponent> collider)
      {
          Lock(playerComponent)->CollideWith(CollisionState::Exit, collider);
      });

    auto shootComponent = CreateActorComponent<ShootComponent>("Shoot");
    shootComponent->SetExtentX(1.f);

    auto input = _playerController->GetInputComponent();
    input->BindAction(
      "Ground", "MoveLeft", 'A', Raw(_playerComponent), &PlayerComponent::HandleInput);
    input->BindAction(
      "Ground", "MoveRight", 'D', Raw(_playerComponent), &PlayerComponent::HandleInput);
    input->BindAction("Ground", "Jump", 'T', Raw(_playerComponent), &PlayerComponent::HandleInput);
    input->BindAction("Ground", "ShootGround", 'R', this, &SnowbrosPlayer::OnShootButtonEvent);
    input->BindAction(
      "Midair", "MoveLeft", 'A', Raw(_playerComponent), &PlayerComponent::HandleInput);
    input->BindAction(
      "Midair", "MoveRight", 'D', Raw(_playerComponent), &PlayerComponent::HandleInput);
    input->BindAction("Midair", "ShootMidair", 'R', this, &SnowbrosPlayer::OnShootButtonEvent);
    input->BindAction(
      "Snowball", "Jump", 'T', Raw(_playerComponent), &PlayerComponent::HandleInput);

    auto kinematic = CreateActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");
    kinematic->SetCollider(_collider);
    kinematic->SetTilemap(tilemap);
    kinematic->RegisterOnStateChangedCallback(PlatformerKinematicState::OnGround,
      [playerComponent = Weak(_playerComponent)]()
      {
          Lock(playerComponent)->Transition(PlayerStateGround::instance);
      });
    kinematic->RegisterOnStateChangedCallback(PlatformerKinematicState::OnAir,
      [playerComponent = Weak(_playerComponent)]()
      {
          Lock(playerComponent)->Transition(New<PlayerStateMidair>(false));
      });
    kinematic->RegisterOnStateChangedCallback(PlatformerKinematicState::OnStandable,
      [playerComponent = Weak(_playerComponent)]()
      {
          Lock(playerComponent)->Transition(PlayerStateGround::instance);
      });

    auto movement = CreateActorComponent<PlatformerMovementComponent>("Movement");
    movement->SetKinematic(kinematic);

    _playerComponent->Transition(PlayerStateGround::instance);

    return true;
}

void SnowbrosPlayer::OnShootButtonEvent(
  Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    auto level            = GetLevel();
    auto collisionManager = level->GetCollisionManager();

    auto movement  = FindActorComponent<PlatformerMovementComponent>("Movement");
    auto kinematic = FindActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");

    auto sprite    = Cast<SceneComponent, SpriteInstanceComponent>(_root);
    auto animation = sprite->GetAnimation();

    std::vector<Weak<AABBCollisionComponent>> snowballs;
    SnowballMorphableEnemyStateMachine::FindSnowballs(collisionManager, snowballs);

    float direction = movement->GetDirection();
    for (auto& snowball : snowballs)
    {
        bool canThrowSnowballBelow = CollisionSystem::AABBToPoint(snowball, _footCollider);
        if (canThrowSnowballBelow)
        {
            auto otherCollider = Lock(snowball);

            auto otherPawn  = Cast<Actor, Pawn>(otherCollider->GetOwner());
            auto otherActor = otherPawn->GetController();

            auto otherAI              = otherActor->FindActorComponent<AIComponent>("AI");
            auto snowballStateMachine = Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(
              otherAI->GetAIStateMachine());

            if (action->GetName() == "ShootGround")
                animation->ChangeAnimationClip("player_kick");
            else if (action->GetName() == "ShootMidair")
                animation->ChangeAnimationClip("player_kick_midair");

            snowballStateMachine->Throw(movement->GetDirection());
            return;
        }

        bool canThrowSnowballRightside
          = direction > 0.f && CollisionSystem::AABBToPoint(snowball, _handColliderRight);
        bool canThrowSnowballLeftside
          = direction < 0.f && CollisionSystem::AABBToPoint(snowball, _handColliderLeft);

        if (canThrowSnowballRightside || canThrowSnowballLeftside)
        {
            auto otherCollider = Lock(snowball);

            auto otherPawn  = Cast<Actor, Pawn>(otherCollider->GetOwner());
            auto otherActor = otherPawn->GetController();

            auto otherAI              = otherActor->FindActorComponent<AIComponent>("AI");
            auto snowballStateMachine = Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(
              otherAI->GetAIStateMachine());

            if (action->GetName() == "ShootGround")
                animation->ChangeAnimationClip("player_kick");
            else if (action->GetName() == "ShootMidair")
                animation->ChangeAnimationClip("player_kick_midair");

            snowballStateMachine->Throw(direction > 0.f ? 1.f : -1.f);
            return;
        }
    }

    auto shootComponent = FindActorComponent<ShootComponent>("Shoot");
    shootComponent->HandleInput(action, buttonEvent);
}
