#include "pch.h"

#include "MonkeyStateMachine.h"

#include "MonkeyBlackboard.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "SnowbrosLevel.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Player/Player.h"
#include "Core/Animation/SpriteInstanceComponent.h"
#include "Tilemap/Tilemap.h"

bool MonkeyStateMachine::Init(Ptr<class AIComponent> owner)
{
    auto blackboard = CreateBlackboard<MonkeyBlackboard>();
    SnowballMorphableEnemyStateMachine::Init(owner);

    auto pawn      = owner->GetPawn<SnowbrosEnemy>();
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    auto sprite    = pawn->FindSceneComponent<SpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();
    animation->ChangeAnimationClip("monkey_walk");

    auto level   = Cast<Level, SnowbrosLevel>(pawn->GetLevel());
    auto tilemap = level->GetTilemap();

    blackboard->walkSpeedX = 1.5f;

    auto enemyStateWalk = FindAIState<SnowbrosEnemyState>("Walk");
    auto enemyStateJump = FindAIState<SnowbrosEnemyState>("Jump");
    auto enemyStateTurn = FindAIState<SnowbrosEnemyState>("Turn");

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
    auto conditionIsPlayerAbove   = CreateAICondition("IsPlayerAbove", ConditionOperator::And,
        [weakLevel = Weak(level), weakPawn = Weak(pawn)](float deltaTime) -> bool
        {
          auto level = Lock(weakLevel);
          auto pawn  = Lock(weakPawn);

          Ptr<Player> player = level->FindNearestPlayerFrom(pawn->GetWorldPosition());
          if (nullptr == player)
              return false;

          float playerPositionY = player->GetWorldPosition().y;
          float thisPositionY   = pawn->GetWorldPosition().y;

          return playerPositionY > thisPositionY && playerPositionY - thisPositionY > 0.9f;
      });
    auto conditionHasLandingTileForward
      = CreateAICondition("HasLandingTileForward", ConditionOperator::And,
        [weakTilemap = Weak(tilemap), weakBlackboard = Weak(blackboard), weakPawn = Weak(pawn)](
          float deltaTime) -> bool
        {
            auto tilemap    = Lock(weakTilemap);
            auto pawn       = Lock(weakPawn);
            auto blackboard = Lock(weakBlackboard);

            Ptr<Tile> tile           = tilemap->GetTile(pawn->GetWorldPosition().ToVector2());
            Vector2   targetPosition = tile->GetPosition();

            targetPosition.x += blackboard->direction * tile->GetSize().x;
            targetPosition.y += tile->GetSize().y;
            Ptr<Tile> targetTile = tilemap->GetTileLocal(targetPosition);

            if (targetTile->IsTopBlock())
            {
                targetPosition.y += tile->GetSize().y;
                blackboard->jumpTargetDirection = targetPosition - tile->GetPosition();
                return true;
            }

            targetPosition.y -= tile->GetSize().y;
            targetTile = tilemap->GetTileLocal(targetPosition);

            if (targetTile->IsTopBlock())
            {
                targetPosition.y += tile->GetSize().y;
                blackboard->jumpTargetDirection = targetPosition - tile->GetPosition();
                return true;
            }

            return false;
        });
    auto conditionHasLandingTileAbove
      = CreateAICondition("HasLandingTileAbove", ConditionOperator::And,
        [weakTilemap = Weak(tilemap), weakBlackboard = Weak(blackboard), weakPawn = Weak(pawn)](
          float deltaTime) -> bool
        {
            auto tilemap    = Lock(weakTilemap);
            auto pawn       = Lock(weakPawn);
            auto blackboard = Lock(weakBlackboard);

            Ptr<Tile> tile           = tilemap->GetTile(pawn->GetWorldPosition().ToVector2());
            Vector2   targetPosition = tile->GetPosition();

            targetPosition.y += tile->GetSize().y;
            Ptr<Tile> targetTile = tilemap->GetTileLocal(targetPosition);

            if (targetTile->IsTopBlock())
            {
                targetPosition.y += tile->GetSize().y;
                blackboard->jumpTargetDirection = targetPosition - tile->GetPosition();
                return true;
            }

            return false;
        });
    auto conditionShouldJumpNext = CreateAICompositeCondition("ShouldJumpNext",
      ConditionOperator::And, conditionMoveAgainstWall, conditionHasLandingTileForward);
    auto conditionCanJumpAbove   = CreateAICompositeCondition(
      "CanJumpAbove", ConditionOperator::And, conditionIsPlayerAbove, conditionHasLandingTileAbove);
    auto conditionShouldJump = CreateAICompositeCondition(
      "ShouldJump", ConditionOperator::Or, conditionShouldJumpNext, conditionCanJumpAbove);
    auto conditionTouchedBlock = CreateAICompositeCondition("TouchedBlock", ConditionOperator::Or,
      conditionMoveAgainstBoundaryX, conditionMoveAgainstWall);

    enemyStateWalk->CreateAITransition("Walk_Jump", enemyStateJump, conditionShouldJump);
    enemyStateWalk->CreateAITransition("Walk_Turn", enemyStateTurn, conditionTouchedBlock);
    enemyStateTurn->CreateAITransition("Turn_Walk", enemyStateWalk, conditionTurned);

    return true;
}

void MonkeyStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}

void MonkeyStateMachine::ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play)
{
    auto pawn   = GetPawn<SnowbrosEnemy>();
    auto sprite = pawn->FindSceneComponent<SpriteInstanceComponent>("Sprite");

    switch (type)
    {
    case SnowbrosEnemyAnimationType::Walk:
        sprite->ChangeAnimation("monkey_walk");
        break;
    case SnowbrosEnemyAnimationType::Turn:
        sprite->ChangeAnimation("monkey_turn");
        break;
    case SnowbrosEnemyAnimationType::Jump:
        sprite->ChangeAnimation("monkey_jump");
        break;
    case SnowbrosEnemyAnimationType::Midair:
        sprite->ChangeAnimation("monkey_midair");
        break;
    case SnowbrosEnemyAnimationType::Crouch:
        sprite->ChangeAnimation("monkey_crouch");
        break;
    case SnowbrosEnemyAnimationType::Awake:
        sprite->ChangeAnimation("monkey_awake");
        break;
    case SnowbrosEnemyAnimationType::Struggle:
        sprite->ChangeAnimation("monkey_struggle");
        break;
    case SnowbrosEnemyAnimationType::Dizzy:
        sprite->ChangeAnimation("monkey_dizzy");
        break;
    case SnowbrosEnemyAnimationType::Launched:
        sprite->ChangeAnimation("monkey_launched");
        break;
    case SnowbrosEnemyAnimationType::Airborne:
        sprite->ChangeAnimation("monkey_airborne");
        break;
    case SnowbrosEnemyAnimationType::Dead:
        sprite->ChangeAnimation("monkey_dead");
        break;
    case SnowbrosEnemyAnimationType::Standing:
        sprite->ChangeAnimation("monkey_standing");
        break;
    default:
        sprite->ChangeAnimation("monkey_none");
        break;
    }

    play ? sprite->Play() : sprite->Pause();
}

void MonkeyStateMachine::AddNotifyToAnimationClipEnd(
  SnowbrosEnemyAnimationType type, std::function<void()>&& func)
{
    auto pawn      = GetPawn<SnowbrosEnemy>();
    auto sprite    = pawn->FindSceneComponent<SpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();

    switch (type)
    {
    case SnowbrosEnemyAnimationType::Walk:
        sprite->AddNotify(
          "monkey_walk", animation->GetClipFrameCount("monkey_walk"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Turn:
        sprite->AddNotify(
          "monkey_turn", animation->GetClipFrameCount("monkey_turn"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Jump:
        sprite->AddNotify(
          "monkey_jump", animation->GetClipFrameCount("monkey_jump"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Midair:
        sprite->AddNotify(
          "monkey_midair", animation->GetClipFrameCount("monkey_midair"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Crouch:
        sprite->AddNotify(
          "monkey_crouch", animation->GetClipFrameCount("monkey_crouch"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Awake:
        sprite->AddNotify(
          "monkey_awake", animation->GetClipFrameCount("monkey_awake"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Struggle:
        sprite->AddNotify(
          "monkey_struggle", animation->GetClipFrameCount("monkey_struggle"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Dizzy:
        sprite->AddNotify(
          "monkey_dizzy", animation->GetClipFrameCount("monkey_dizzy"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Launched:
        sprite->AddNotify(
          "monkey_launched", animation->GetClipFrameCount("monkey_launched"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Airborne:
        sprite->AddNotify(
          "monkey_airborne", animation->GetClipFrameCount("monkey_airborne"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Dead:
        sprite->AddNotify(
          "monkey_dead", animation->GetClipFrameCount("monkey_dead"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Standing:
        sprite->AddNotify(
          "monkey_standing", animation->GetClipFrameCount("monkey_standing"), std::move(func));
        break;
    default:
        break;
    }
}
