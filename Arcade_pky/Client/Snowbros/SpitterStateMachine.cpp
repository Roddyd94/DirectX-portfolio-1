#include "pch.h"

#include "SpitterStateMachine.h"

#include "Core/TimeManager.h"

#include "EnemyProjectile.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "SnowbrosLevel.h"
#include "SpitterBlackboard.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Level.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Player/Player.h"
#include "Snowbros/IndexedSpriteInstanceComponent.h"
#include "Tilemap/Tilemap.h"

bool SpitterStateMachine::Init(Ptr<class AIComponent> owner)
{
    auto blackboard = CreateBlackboard<SpitterBlackboard>();
    SnowballMorphableEnemyStateMachine::Init(owner);

    auto pawn      = owner->GetPawn<SnowbrosEnemy>();
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    auto sprite    = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();
    animation->ChangeAnimationClip("spitter_walk");

    auto level   = Cast<Level, SnowbrosLevel>(pawn->GetLevel());
    auto tilemap = level->GetTilemap();

    auto enemyStateWalk  = FindAIState<SnowbrosEnemyState>("Walk");
    auto enemyStateJump  = FindAIState<SnowbrosEnemyState>("Jump");
    auto enemyStateTurn  = FindAIState<SnowbrosEnemyState>("Turn");
    auto enemyStateShoot = CreateAIState<SnowbrosEnemyState>("Shoot", SnowbrosEnemyState::Fire);

    enemyStateShoot->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn();
          auto level      = pawn->GetLevel();
          auto blackboard = GetBlackboard<SpitterBlackboard>();

          auto projectile = level->SpawnActor<EnemyProjectile>(
            pawn->GetWorldPosition(), Vector3::one * 2.f, Vector3::zero);
          projectile->SetDirection(blackboard->direction);

          auto sprite = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
          sprite->ChangeAnimation("spitter_shoot");

          blackboard->transitionTimerID = TimeManager::Instance().SetTimer(1.f, false,
            [this]()
            {
                if (GetCurrentState()->GetName() == "Shoot")
                    Transition("Walk");
            });
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
    auto conditionNextPatrolPointAbove
      = CreateAICondition("NextPatrolPointAbove", ConditionOperator::And,
        [weakTilemap = Weak(tilemap), weakBlackboard = Weak(blackboard), weakPawn = Weak(pawn)](
          float deltaTime) -> bool
        {
            auto tilemap    = Lock(weakTilemap);
            auto pawn       = Lock(weakPawn);
            auto blackboard = Lock(weakBlackboard);

            if (blackboard->currentPatrolIndex >= blackboard->patrolPoints.size())
            {
                blackboard->currentPatrolIndex = 0;
                --blackboard->patrolLoopCount;
            }

            Vector2 pawnPosition = pawn->GetWorldPosition().ToVector2();
            Vector2 targetPoint  = blackboard->patrolPoints[blackboard->currentPatrolIndex];

            if (targetPoint.y > pawnPosition.y + 0.6f)
                return true;

            return false;
        });
    auto conditionShouldJumpNext           = CreateAICompositeCondition("ShouldJumpNext",
      ConditionOperator::And, conditionMoveAgainstWall, conditionHasLandingTileForward);
    auto conditionCanJumpAboveTowardPlayer = CreateAICompositeCondition(
      "CanJumpAboveTowardPlayer", ConditionOperator::And, conditionIsPlayerAbove, conditionHasLandingTileAbove);
    auto conditionCanJumpAboveTowardNextPatrolPoint = CreateAICompositeCondition(
      "CanJumpAboveTowardNextPatrolPoint", ConditionOperator::And, conditionNextPatrolPointAbove, conditionHasLandingTileAbove);
    auto conditionShouldJump   = CreateAICompositeCondition("ShouldJump", ConditionOperator::Or,
      conditionCanJumpAboveTowardNextPatrolPoint, conditionShouldJumpNext, conditionCanJumpAboveTowardPlayer);
    auto conditionTouchedBlock = CreateAICompositeCondition("TouchedBlock", ConditionOperator::Or,
      conditionMoveAgainstBoundaryX, conditionMoveAgainstWall);

    enemyStateWalk->CreateAITransition("Walk_Jump", enemyStateJump, conditionShouldJump);
    enemyStateWalk->CreateAITransition("Walk_Turn", enemyStateTurn, conditionTouchedBlock);
    enemyStateTurn->CreateAITransition("Turn_Shoot", enemyStateShoot, conditionTurned);

    return true;
}

void SpitterStateMachine::Destroy()
{
    auto blackboard = CreateBlackboard<SpitterBlackboard>();
    TimeManager::Instance().RemoveTimer(blackboard->transitionTimerID);

    AIStateMachine::Destroy();
}

void SpitterStateMachine::ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play)
{
    auto pawn   = GetPawn<SnowbrosEnemy>();
    auto sprite = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

    switch (type)
    {
    case SnowbrosEnemyAnimationType::Walk:
        sprite->ChangeAnimation("spitter_walk");
        break;
    case SnowbrosEnemyAnimationType::Turn:
        sprite->ChangeAnimation("spitter_turn");
        break;
    case SnowbrosEnemyAnimationType::Jump:
        sprite->ChangeAnimation("spitter_jump");
        break;
    case SnowbrosEnemyAnimationType::Midair:
        sprite->ChangeAnimation("spitter_midair");
        break;
    case SnowbrosEnemyAnimationType::Crouch:
        sprite->ChangeAnimation("spitter_crouch");
        break;
    case SnowbrosEnemyAnimationType::Awake:
        sprite->ChangeAnimation("spitter_awake");
        break;
    case SnowbrosEnemyAnimationType::Struggle:
        sprite->ChangeAnimation("spitter_struggle");
        break;
    case SnowbrosEnemyAnimationType::Dizzy:
        sprite->ChangeAnimation("spitter_dizzy");
        break;
    case SnowbrosEnemyAnimationType::Launched:
        sprite->ChangeAnimation("spitter_launched");
        break;
    case SnowbrosEnemyAnimationType::Airborne:
        sprite->ChangeAnimation("spitter_airborne");
        break;
    case SnowbrosEnemyAnimationType::Dead:
        sprite->ChangeAnimation("spitter_dead");
        break;
    case SnowbrosEnemyAnimationType::Standing:
        sprite->ChangeAnimation("spitter_standing");
        break;
    case SnowbrosEnemyAnimationType::Shoot:
        sprite->ChangeAnimation("spitter_shoot");
        break;
    default:
        sprite->ChangeAnimation("spitter_none");
        break;
    }

    play ? sprite->Play() : sprite->Pause();
}

void SpitterStateMachine::AddNotifyToAnimationClipEnd(
  SnowbrosEnemyAnimationType type, std::function<void()>&& func)
{
    auto pawn      = GetPawn<SnowbrosEnemy>();
    auto sprite    = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();

    switch (type)
    {
    case SnowbrosEnemyAnimationType::Walk:
        sprite->AddNotify(
          "spitter_walk", animation->GetClipFrameCount("spitter_walk"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Turn:
        sprite->AddNotify(
          "spitter_turn", animation->GetClipFrameCount("spitter_turn"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Jump:
        sprite->AddNotify(
          "spitter_jump", animation->GetClipFrameCount("spitter_jump"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Midair:
        sprite->AddNotify(
          "spitter_midair", animation->GetClipFrameCount("spitter_midair"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Crouch:
        sprite->AddNotify(
          "spitter_crouch", animation->GetClipFrameCount("spitter_crouch"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Awake:
        sprite->AddNotify(
          "spitter_awake", animation->GetClipFrameCount("spitter_awake"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Struggle:
        sprite->AddNotify(
          "spitter_struggle", animation->GetClipFrameCount("spitter_struggle"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Dizzy:
        sprite->AddNotify(
          "spitter_dizzy", animation->GetClipFrameCount("spitter_dizzy"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Launched:
        sprite->AddNotify(
          "spitter_launched", animation->GetClipFrameCount("spitter_launched"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Airborne:
        sprite->AddNotify(
          "spitter_airborne", animation->GetClipFrameCount("spitter_airborne"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Dead:
        sprite->AddNotify(
          "spitter_dead", animation->GetClipFrameCount("spitter_dead"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Standing:
        sprite->AddNotify(
          "spitter_standing", animation->GetClipFrameCount("spitter_standing"), std::move(func));
        break;
    case SnowbrosEnemyAnimationType::Shoot:
        sprite->AddNotify(
          "spitter_shoot", animation->GetClipFrameCount("spitter_shoot"), std::move(func));
        break;
    default:
        break;
    }
}
