#include "pch.h"

#include "SpawnStateMachine.h"

#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "SnowbrosLevel.h"
#include "SpawnBlackboard.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Core/Animation/SpriteInstanceComponent.h"

bool SpawnStateMachine::Init(Ptr<class AIComponent> owner)
{
    auto blackboard = CreateBlackboard<SpawnBlackboard>();
    SnowballMorphableEnemyStateMachine::Init(owner);

    auto pawn      = owner->GetPawn<SnowbrosEnemy>();
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    auto sprite    = pawn->FindSceneComponent<SpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();
    animation->ChangeAnimationClip("boss_spawn_fly");

    kinematic->SetCanEscapeBoundaryX(true);

    blackboard->walkSpeedX = 3.f;

    auto enemyStateWalk = FindAIState<SnowbrosEnemyState>("Walk");
    enemyStateWalk->SetInterval(0.1f);
    auto enemyStateTurn     = FindAIState<SnowbrosEnemyState>("Turn");
    auto enemyStateCrouch   = FindAIState<SnowbrosEnemyState>("Crouch");
    auto enemyStateFall     = FindAIState<SnowbrosEnemyState>("Fall");
    auto enemyStateLaunched = FindAIState<SnowbrosEnemyState>("Launched");
    auto enemyStateDead     = FindAIState<SnowbrosEnemyState>("Dead");
    auto enemyStateFly      = CreateAIState<SnowbrosEnemyState>("Fly", SnowbrosEnemyState::Fly);

    enemyStateFly->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<SpawnBlackboard>();
          auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
          auto sprite     = pawn->FindSceneComponent<SpriteInstanceComponent>("Sprite");

          sprite->ChangeAnimation("boss_spawn_midair");
          kinematic->SetVelocity(blackboard->initialVelocity);
      });
    enemyStateLaunched->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          Transition("Dead");
      });
    enemyStateDead->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          auto pawn  = GetPawn();
          auto level = Cast<Level, SnowbrosLevel>(pawn->GetLevel());
          pawn->SetActive(false);

          auto item = level->SpawnItem(pawn->GetWorldPosition(), Item::Sushi);
          item->SetItemNumber(1);
      });

    enemyStateFly->RegisterCallback(AIEventState::Tick,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<SpawnBlackboard>();
          auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

          Vector3 position = pawn->GetWorldPosition();
          Vector2 velocity = kinematic->GetVelocity();

          if (position.x < blackboard->landPositionX)
          {
              velocity.x += blackboard->dragX;
              if (velocity.x > 0.f)
              {
                  velocity.x = 0.f;
                  Transition("Fall");
              }

              kinematic->SetVelocity(velocity);
          }

          // todo  Drag & Gravity
      });

    auto conditionTurned               = CreateAICondition("Turned", ConditionOperator::And,
                    [weakBlackboard = Weak(blackboard)](float deltaTime) -> bool
                    {
          auto blackboard = Lock(weakBlackboard);
          return blackboard->turned;
      });
    auto conditionMoveAgainstBoundaryX = CreateAICondition("MoveAgainstBoundaryX",
      ConditionOperator::And,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime) -> bool
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          float deltaX = kinematic->GetVelocity().x * deltaTime;

          return kinematic->IsColliderMovingAgainstBoundaryX(deltaX);
      });
    auto conditionMoveAgainstWall = CreateAICondition("MoveAgainstWall", ConditionOperator::And,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime) -> bool
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          float deltaX = kinematic->GetVelocity().x * deltaTime;

          return kinematic->IsColliderMovingAgainstWallX(deltaX);
      });
    auto conditionNotOnFirstFloor = CreateAICondition("NotOnFirstFloor", ConditionOperator::And,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime) -> bool
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          return !kinematic->IsColliderOnFirstFloor();
      });
    auto conditionOutOfLevel      = CreateAICondition("OutOfLevel", ConditionOperator::And,
           [this](float deltaTime) -> bool
           {
          auto pawn  = GetPawn();
          auto level = Cast<Level, SnowbrosLevel>(pawn->GetLevel());

          Rect boundary = level->GetBoundary();
          boundary.left -= 2.f;
          boundary.right += 2.f;
          boundary.bottom -= 2.f;
          boundary.top += 2.f;

          Vector3 position = pawn->GetWorldPosition();

          return position.x < boundary.left || position.x > boundary.right
              || position.y < boundary.bottom || position.y > boundary.top;
      });
    auto conditionTouchedBlock = CreateAICompositeCondition("TouchedBlock", ConditionOperator::Or,
      conditionMoveAgainstBoundaryX, conditionMoveAgainstWall);
    auto conditionTouchedBlockNotOnFirstFloor
      = CreateAICompositeCondition("TouchedBlockNotOnFirstFloor", ConditionOperator::And,
        conditionMoveAgainstBoundaryX, conditionNotOnFirstFloor);

    enemyStateTurn->CreateAITransition("Turn_Crouch", enemyStateCrouch, conditionTurned);
    enemyStateWalk->CreateAITransition(
      "Walk_Turn", enemyStateTurn, conditionTouchedBlockNotOnFirstFloor);
    enemyStateFall->CreateAITransition("Fall_Dead", enemyStateDead, conditionOutOfLevel);

    Transition("Fly");

    return true;
}

void SpawnStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}

void SpawnStateMachine::ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play)
{
    auto pawn   = GetPawn<SnowbrosEnemy>();
    auto sprite = pawn->FindSceneComponent<SpriteInstanceComponent>("Sprite");

    switch (type)
    {
    case SnowbrosEnemyAnimationType::Walk:
        sprite->ChangeAnimation("boss_spawn_walk");
        break;
    case SnowbrosEnemyAnimationType::Turn:
        sprite->ChangeAnimation("boss_spawn_turn");
        break;
    case SnowbrosEnemyAnimationType::Midair:
        sprite->ChangeAnimation("boss_spawn_midair");
        break;
    case SnowbrosEnemyAnimationType::Crouch:
    case SnowbrosEnemyAnimationType::Awake:
        sprite->ChangeAnimation("boss_spawn_awake");
        break;
    case SnowbrosEnemyAnimationType::Struggle:
        sprite->ChangeAnimation("boss_spawn_struggle");
        break;
    }

    play ? sprite->Play() : sprite->Pause();
}

void SpawnStateMachine::AddNotifyToAnimationClipEnd(
  SnowbrosEnemyAnimationType type, std::function<void()>&& func)
{
    auto pawn      = GetPawn<SnowbrosEnemy>();
    auto sprite    = pawn->FindSceneComponent<SpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();

    switch (type)
    {
    case SnowbrosEnemyAnimationType::Walk:
        sprite->AddNotify(
          "boss_spawn_walk", animation->GetClipFrameCount("boss_spawn_walk"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Turn:
        sprite->AddNotify(
          "boss_spawn_turn", animation->GetClipFrameCount("boss_spawn_turn"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Fly:
    case SnowbrosEnemyAnimationType::Midair:
        sprite->AddNotify(
          "boss_spawn_midair", animation->GetClipFrameCount("boss_spawn_midair"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Crouch:
    case SnowbrosEnemyAnimationType::Awake:
        sprite->AddNotify(
          "boss_spawn_awake", animation->GetClipFrameCount("boss_spawn_awake"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Struggle:
        sprite->AddNotify("boss_spawn_struggle",
          animation->GetClipFrameCount("boss_spawn_struggle"), std::move(func));
        break;
    }
}
