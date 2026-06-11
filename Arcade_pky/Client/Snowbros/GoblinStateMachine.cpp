#include "pch.h"

#include "GoblinStateMachine.h"

#include "GoblinBlackboard.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "SnowbrosLevel.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Player/Player.h"
#include "Snowbros/IndexedSpriteInstanceComponent.h"
#include "Tilemap/Tilemap.h"

bool GoblinStateMachine::Init(Ptr<class AIComponent> owner)
{
    auto blackboard = CreateBlackboard<GoblinBlackboard>();
    SnowballMorphableEnemyStateMachine::Init(owner);

    auto pawn      = owner->GetPawn<SnowbrosEnemy>();
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    auto sprite    = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();
    animation->ChangeAnimationClip("goblin_walk");

    auto level   = Cast<Level, SnowbrosLevel>(pawn->GetLevel());
    auto tilemap = level->GetTilemap();

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

          Ptr<Player> player = level->GetPlayer();
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

void GoblinStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}

void GoblinStateMachine::ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play)
{
    auto pawn   = GetPawn<SnowbrosEnemy>();
    auto sprite = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

    switch (type)
    {
    case SnowbrosEnemyAnimationType::Walk:
        sprite->ChangeAnimation("goblin_walk");
        break;
    case SnowbrosEnemyAnimationType::Turn:
        sprite->ChangeAnimation("goblin_turn");
        break;
    case SnowbrosEnemyAnimationType::Jump:
        sprite->ChangeAnimation("goblin_jump");
        break;
    case SnowbrosEnemyAnimationType::Midair:
        sprite->ChangeAnimation("goblin_midair");
        break;
    case SnowbrosEnemyAnimationType::Crouch:
        sprite->ChangeAnimation("goblin_crouch");
        break;
    case SnowbrosEnemyAnimationType::Awake:
        sprite->ChangeAnimation("goblin_awake");
        break;
    case SnowbrosEnemyAnimationType::Struggle:
        sprite->ChangeAnimation("goblin_struggle");
        break;
    case SnowbrosEnemyAnimationType::Dizzy:
        sprite->ChangeAnimation("goblin_dizzy");
        break;
    case SnowbrosEnemyAnimationType::Launched:
        sprite->ChangeAnimation("goblin_launched");
        break;
    case SnowbrosEnemyAnimationType::Airborne:
        sprite->ChangeAnimation("goblin_airborne");
        break;
    case SnowbrosEnemyAnimationType::Dead:
        sprite->ChangeAnimation("goblin_dead");
        break;
    case SnowbrosEnemyAnimationType::Standing:
        sprite->ChangeAnimation("goblin_standing");
        break;
    default:
        sprite->ChangeAnimation("goblin_none");
        break;
    }

    play ? sprite->Play() : sprite->Pause();
}

void GoblinStateMachine::AddNotifyToAnimationClipEnd(
  SnowbrosEnemyAnimationType type, std::function<void()>&& func)
{
    auto pawn      = GetPawn<SnowbrosEnemy>();
    auto sprite    = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();

    switch (type)
    {
    case SnowbrosEnemyAnimationType::Walk:
        sprite->AddNotify(
          "goblin_walk", animation->GetClipFrameCount("goblin_walk"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Turn:
        sprite->AddNotify(
          "goblin_turn", animation->GetClipFrameCount("goblin_turn"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Jump:
        sprite->AddNotify(
          "goblin_jump", animation->GetClipFrameCount("goblin_jump"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Midair:
        sprite->AddNotify(
          "goblin_midair", animation->GetClipFrameCount("goblin_midair"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Crouch:
        sprite->AddNotify(
          "goblin_crouch", animation->GetClipFrameCount("goblin_crouch"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Awake:
        sprite->AddNotify(
          "goblin_awake", animation->GetClipFrameCount("goblin_awake"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Struggle:
        sprite->AddNotify(
          "goblin_struggle", animation->GetClipFrameCount("goblin_struggle"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Dizzy:
        sprite->AddNotify(
          "goblin_dizzy", animation->GetClipFrameCount("goblin_dizzy"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Launched:
        sprite->AddNotify(
          "goblin_launched", animation->GetClipFrameCount("goblin_launched"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Airborne:
        sprite->AddNotify(
          "goblin_airborne", animation->GetClipFrameCount("goblin_airborne"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Dead:
        sprite->AddNotify(
          "goblin_dead", animation->GetClipFrameCount("goblin_dead"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Standing:
        sprite->AddNotify(
          "goblin_standing", animation->GetClipFrameCount("goblin_standing"), std::move(func));
        break;
    default:
        break;
    }
}
