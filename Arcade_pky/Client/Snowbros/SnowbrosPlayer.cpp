#include "pch.h"

#include "SnowbrosPlayer.h"

#include "Core/Collision/CollisionManager.h"
#include "Core/ResourceManager.h"
#include "Core/TimeManager.h"

#include "PlayerStateClear.h"
#include "PlayerStateGround.h"
#include "PlayerStateMidair.h"
#include "PlayerStateSpawned.h"
#include "ShootComponent.h"
#include "SnowballMorphableEnemyStateMachine.h"
#include "SnowbrosLevel.h"
#include "SnowbrosPlayerBlackboard.h"
#include "Types.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Animation/SpriteInstanceComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Collision/PointCollisionComponent.h"
#include "Core/Input/InputComponent.h"
#include "Core/Palette.h"
#include "Platformer/PlatformerKinematicPlayerComponent.h"
#include "Platformer/PlatformerMovementComponent.h"
#include "Player/PlayerComponent.h"
#include "Player/PlayerController.h"
#include "Snowbros/IndexedSpriteInstanceComponent.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool SnowbrosPlayer::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    Player::Init(id, position, scale, rotation);
    SetName("Player");

    auto blackboard = New<SnowbrosPlayerBlackboard>();
    _playerComponent->SetBlackboard(blackboard);

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto sprite  = CreateSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto palette = FIND_PALETTE("player_1");
    sprite->SetPaletteNumber(palette->GetID());
    sprite->SetRenderLayer("Player");
    // rootComp->SetShader("SpriteShader");
    sprite->AttachToComponent(_root);
    sprite->SetWorldScale({2.f, 2.f});
    sprite->SetWorldRotation(Vector3::zero);

    Ptr<Animation2D> animation = sprite->CreateAnimation();
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
    animation->AddNotify("player_melt", animation->GetClipFrameCount("player_melt"),
      [this]()
      {
          SetActive(false);
          auto level = Cast<Level, SnowbrosLevel>(GetLevel());
          level->SetPlayer(nullptr);
      });
    animation->AddNotify("player_dead", animation->GetClipFrameCount("player_dead"),
      [this]()
      {
          SetActive(false);
          auto level = Cast<Level, SnowbrosLevel>(GetLevel());
          level->SetPlayer(nullptr);
      });

    auto effect = CreateSceneComponent<IndexedSpriteInstanceComponent>("Effect");
    palette     = FIND_PALETTE("snowball");
    effect->SetPaletteNumber(palette->GetID());
    effect->SetRenderLayer("PlayerEffect");
    effect->AttachToComponent(_root);
    effect->SetWorldScale({3.f, 2.f});
    effect->SetWorldRotation(Vector3::zero);

    animation = effect->CreateAnimation();
    animation->SetAnimationSequence("effect");
    animation->ChangeAnimationClip("effect_eruption");

    animation->AddNotify("effect_eruption", animation->GetClipFrameCount("effect_eruption"),
      [this, weakEffect = Weak(effect)]()
      {
          auto effect = Lock(weakEffect);
          effect->SetWorldScale({2.f, 2.f});
          effect->ChangeAnimation("effect_dust");

          this->GetRoot()->SetEnable(true);
      });
    animation->AddNotify("effect_dust", animation->GetClipFrameCount("effect_dust"),
      [this, weakSprite = Weak(sprite), weakEffect = Weak(effect)]()
      {
          auto effect = Lock(weakEffect);
          auto sprite = Lock(weakSprite);
;
          effect->SetEnable(false);
          sprite->SetEnable(true);

          _playerComponent->Transition(PlayerStateGround::instance);
      });

    _headCollider = CreateSceneComponent<PointCollisionComponent>("HeadCollider");
    _headCollider->AttachToComponent(_root);
    _headCollider->SetCollisionProfile("PlayerHead");
    _headCollider->SetRelativePosition({0.f, 0.25f});

    _handColliderLeft = CreateSceneComponent<PointCollisionComponent>("HandColliderLeft");
    _handColliderLeft->AttachToComponent(_root);
    _handColliderLeft->SetCollisionProfile("PlayerHandLeft");
    _handColliderLeft->SetRelativePosition({-0.26f, -0.1875f});

    _handColliderRight = CreateSceneComponent<PointCollisionComponent>("HandColliderRight");
    _handColliderRight->AttachToComponent(_root);
    _handColliderRight->SetCollisionProfile("PlayerHandRight");
    _handColliderRight->SetRelativePosition({0.26f, -0.1875f});

    _footCollider = CreateSceneComponent<PointCollisionComponent>("FootCollider");
    _footCollider->AttachToComponent(_root);
    _footCollider->SetCollisionProfile("PlayerFoot");
    _footCollider->SetRelativePosition({0.f, -0.26f});

    _collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    _collider->AttachToComponent(_root);
    _collider->SetBoxSize({0.9f, 1.2f});
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

    InputSystem::Instance().FindOrAddInputContext("None");
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

void SnowbrosPlayer::SetDirection(float direction)
{
    auto movement = FindActorComponent<PlatformerMovementComponent>("Movement");
    movement->SetDirection(direction);

    auto sprite = FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    sprite->SetFlipX(direction > 0 ? true : false);
}

void SnowbrosPlayer::ResetState()
{
    auto stateMachine = _playerComponent->GetStateMachine();
    stateMachine->Transition(PlayerStateGround::instance);
}

void SnowbrosPlayer::StartStage()
{
    _playerComponent->Transition(New<PlayerStateSpawned>());
}

void SnowbrosPlayer::EndStage()
{
    _playerComponent->Transition(PlayerStateClear::instance);
}

void SnowbrosPlayer::OnShootButtonEvent(
  Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
{
    auto level            = GetLevel();
    auto collisionManager = level->GetCollisionManager();

    auto movement  = FindActorComponent<PlatformerMovementComponent>("Movement");
    auto kinematic = FindActorComponent<PlatformerKinematicPlayerComponent>("Kinematic");

    auto sprite    = FindSceneComponent<SpriteInstanceComponent>("Sprite");
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
