#include "pch.h"

#include "GoblinStateMachine.h"

#include "Core/Collision/CollisionManager.h"
#include "Core/TimeManager.h"

#include "GoblinBlackboard.h"
#include "SnowProjectile.h"
#include "SnowProjectileComponent.h"
#include "SnowbrosLevel.h"
#include "AI/AIComponent.h"
#include "Core/Actor.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Player/Player.h"
#include "Tilemap/Tilemap.h"

void GoblinStateMachine::Init(Ptr<class AIComponent> owner)
{
    AIStateMachine::Init(owner);
    auto blackboard = CreateBlackboard<GoblinBlackboard>();

    auto actor   = owner->GetOwner();
    auto level   = Cast<Level, SnowbrosLevel>(owner->GetLevel());
    auto tilemap = level->GetTilemap();

    auto collider  = actor->FindSceneComponent<AABBCollisionComponent>("Collider");
    auto kinematic = actor->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    auto sprite            = actor->FindSceneComponent<SpriteComponent>("Sprite");
    auto animation         = sprite->CreateAnimation();
    auto animationSequence = animation->GetSequence();
    animation->SetAnimationSequence("goblin");
    animation->ChangeAnimationClip("goblin_walk");

    auto snowballSprite            = actor->FindSceneComponent<SpriteComponent>("Snowball");
    auto snowballAnimation         = snowballSprite->CreateAnimation();
    auto snowballAnimationSequence = snowballAnimation->GetSequence();
    snowballAnimation->SetAnimationSequence("snowball");
    snowballAnimation->ChangeAnimationClip("snowball_none");

#pragma region ColliderCallbacks
    collider->RegisterCollisionCallBack(CollisionState::Enter,
      [=, thisCollider = collider](Weak<CollisionComponent> collider)
      {
          Ptr<CollisionComponent> otherCollider     = Lock(collider);
          ColliderType::Type      otherColliderType = otherCollider->GetColliderType();

          switch (otherColliderType)
          {
          case ColliderType::PlayerHead:
          {
              if (_currentState->GetName() == "Snowball")
              {
                  Rect    thisColliderRect = thisCollider->GetBox();
                  Vector2 playerPosition   = otherCollider->GetWorldPosition().ToVector2();

                  auto player = otherCollider->GetOwner();
                  auto playerKinematic
                    = player->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

                  if (playerKinematic->GetVelocity().y <= 0.f && kinematic->GetVelocity().y >= 0.f)
                      break;

                  float direction
                    = playerPosition.x < thisCollider->GetWorldPosition().x ? 1.f : -1.f;
                  kinematic->AddForce(
                    {direction * blackboard->snowballHeadingForceX, blackboard->jumpForceFull});
              }
          }
          break;
          case ColliderType::PlayerProjectile:
          {
              auto otherActor = Cast<Actor, SnowProjectile>(otherCollider->GetOwner());
              auto projectile = otherActor->GetProjectileComponent();

              if (projectile->IsHit())
                  break;

              if (_currentState->GetName() == "Snowball")
                  blackboard->accTime += blackboard->snowballIncFormed;
              else if (_currentState->GetName() == "Struggle")
                  blackboard->accTime += blackboard->snowballIncForming;
              else
              {
                  Transition("Struggle");
                  blackboard->accTime = blackboard->snowballFormingInitialValue;
              }
          }
          break;
          case ColliderType::Enemy:
          {
              if (_currentState->GetName() == "Snowball")
                  ;
          }
          break;
          case ColliderType::EnemyProjectile:
              break;
          case ColliderType::Item:
              break;
          case ColliderType::End:
              break;
          default:
              break;
          }
      });
    collider->RegisterCollisionCallBack(CollisionState::Stay,
      [=](Weak<CollisionComponent> collider)
      {
          Ptr<CollisionComponent> otherCollider     = Lock(collider);
          ColliderType::Type      otherColliderType = otherCollider->GetColliderType();

          switch (otherColliderType)
          {
          case ColliderType::Enemy:
          {
              auto thisPosition  = GetOwner()->GetOwner()->GetWorldPosition();
              auto otherPosition = otherCollider->GetWorldPosition();

              auto otherEnemy = otherCollider->GetOwner();
              auto otherAI    = otherEnemy->FindActorComponent<AIComponent>("AI");

              auto otherStateMachine = otherAI->GetAIStateMachine();
              auto otherState        = otherStateMachine->GetCurrentState();

              if (_currentState->GetName() == "Walk")
              {
                  if (otherState->GetName() != "Walk" && otherState->GetName() != "Struggle")
                      return;

                  if (thisPosition.x < otherPosition.x && blackboard->direction < 0)
                      return;

                  if (thisPosition.x > otherPosition.x && blackboard->direction > 0)
                      return;

                  Transition("Turn");
              }
              else if (_currentState->GetName() == "Snowball")
              {
                  if (otherState->GetName() != "Snowball")
                      return;

                  auto otherSnowballStateMachine
                    = Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(otherStateMachine);

                  float distance = std::abs(thisPosition.x - otherPosition.x);
                  if (distance > blackboard->snowballRepulsiveDistance)
                      return;

                  if (thisPosition.x < otherPosition.x)
                  {
                      otherSnowballStateMachine->TryMoveX(blackboard->snowballRepulsiveDeltaX);
                      TryMoveX(-blackboard->snowballRepulsiveDeltaX);
                  }
                  else
                  {
                      otherSnowballStateMachine->TryMoveX(-blackboard->snowballRepulsiveDeltaX);
                      TryMoveX(blackboard->snowballRepulsiveDeltaX);
                  }
              }
          }
          break;
          }
      });
#pragma endregion ColliderCallbacks

#pragma region AnimationNotifies
    animation->AddNotify("goblin_turn", animation->GetClipFrameCount("goblin_turn"),
      [=]()
      {
          blackboard->turned = true;
      });
    animation->AddNotify("goblin_crouch", animation->GetClipFrameCount("goblin_crouch"),
      [=]()
      {
          this->Transition("Walk");
      });
    animation->AddNotify("goblin_jump", animation->GetClipFrameCount("goblin_jump"),
      [=]()
      {
          kinematic->AddForce(blackboard->GetTargetJumpForce());
          blackboard->isJumping = true;
      });
    animation->AddNotify("goblin_awake", animation->GetClipFrameCount("goblin_awake"),
      [=]()
      {
          animation->ChangeAnimationClip("goblin_dizzy");
      });
#pragma endregion AnimationNotifies

#pragma region AIStates
    auto enemyStateStand  = CreateAIState("Stand");
    auto enemyStatePatrol = CreateAIState("Patrol");
    auto enemyStateWalk   = CreateAIState("Walk");
    enemyStateWalk->SetInterval(0.1f);
    auto enemyStateTurn     = CreateAIState("Turn");
    auto enemyStateJump     = CreateAIState("Jump");
    auto enemyStateFall     = CreateAIState("Fall");
    auto enemyStateCrouch   = CreateAIState("Crouch");
    auto enemyStateDizzy    = CreateAIState("Dizzy");
    auto enemyStateStruggle = CreateAIState("Struggle");
    auto enemyStateSnowball = CreateAIState("Snowball");
    auto enemyStateRolling  = CreateAIState("Rolling");
    auto enemyStateLaunched = CreateAIState("Launched");
    auto enemyStateDead     = CreateAIState("Dead");
#pragma endregion AIStates

#pragma region RegisterStateCallbackEnter
    enemyStateWalk->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          animation->ChangeAnimationClip("goblin_walk");
          kinematic->SetVelocityX(blackboard->direction * blackboard->walkSpeedX);
      });
    enemyStateTurn->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          blackboard->turned = false;
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_turn");
      });
    enemyStateJump->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          blackboard->isJumping = false;
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_jump");
      });
    enemyStateFall->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_midair");
      });
    enemyStateCrouch->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          kinematic->AdjustPositionToFloor();
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_crouch");
      });
    enemyStateStruggle->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_struggle");
          snowballSprite->SetEnable(true);
          snowballAnimation->ChangeAnimationClip("snowball_forming", false);
      });
    enemyStateSnowball->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          blackboard->previousPosition = actor->GetWorldPosition().ToVector2();
          blackboard->accTime += blackboard->snowballFormedBonusValue;
          animation->ChangeAnimationClip("goblin_none");
          snowballAnimation->ChangeAnimationClip("snowball_formed", false);
      });
    enemyStateRolling->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          blackboard->hitCount = 0;
          animation->ChangeAnimationClip("goblin_none");
          snowballAnimation->ChangeAnimationClip("snowball_rolling");
      });
    enemyStateDizzy->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_awake");
      });
#pragma endregion RegisterStateCallbackEnter

#pragma region RegisterStateCallbackTick
    enemyStateWalk->RegisterCallback(AIEventState::Tick,
      [=](float deltaTime)
      {
          kinematic->SetVelocity({blackboard->direction * blackboard->walkSpeedX, 0.f});
          blackboard->previousDelta = kinematic->GetVelocity() * deltaTime;
      });
    enemyStateJump->RegisterCallback(AIEventState::Tick,
      [=](float deltaTime)
      {
          if (blackboard->isJumping)
              kinematic->AddGravity(deltaTime);
          blackboard->previousDelta = kinematic->GetVelocity() * deltaTime;
      });
    enemyStateFall->RegisterCallback(AIEventState::Tick,
      [=](float deltaTime)
      {
          kinematic->AddGravity(deltaTime);
          blackboard->previousDelta = kinematic->GetVelocity() * deltaTime;
      });
    enemyStateStruggle->RegisterCallback(AIEventState::Tick,
      [=](float deltaTime)
      {
          Vector2 velocity = kinematic->GetVelocity();

          if (velocity.y < 0.f && kinematic->IsColliderMovingAgainstFloor(velocity * deltaTime))
          {
              kinematic->SetVelocity(Vector2::zero);
              kinematic->AdjustPositionToFloor(velocity * deltaTime);
          }
          else if (!kinematic->IsColliderOnFloor())
          {
              kinematic->AddGravity(deltaTime);
          }

          blackboard->accTime -= blackboard->snowballDecPerSecond * deltaTime;

          if (blackboard->accTime > blackboard->phaseThreshold[3])
              Transition("Snowball");
          else if (blackboard->accTime > blackboard->phaseThreshold[2])
              snowballAnimation->SetFrameIndex(2);
          else if (blackboard->accTime > blackboard->phaseThreshold[1])
              snowballAnimation->SetFrameIndex(1);
          else if (blackboard->accTime > blackboard->phaseThreshold[0])
              snowballAnimation->SetFrameIndex(0);
          else
          {
              Transition("Dizzy");
              blackboard->accTime = 0.f;
          }
      });
    enemyStateSnowball->RegisterCallback(AIEventState::Tick,
      [=](float deltaTime)
      {
          Vector2 currentPosition = actor->GetWorldPosition().ToVector2();
          if (std::abs(currentPosition.x - blackboard->previousPosition.x)
              > blackboard->snowballFrameDistance)
          {
              int32 frameIndex = snowballSprite->GetFrameIndex() + 1;
              int32 frameCount = snowballSprite->GetFrameCount();
              snowballSprite->SetFrameIndex(frameIndex % frameCount);
              blackboard->previousPosition = currentPosition;
          }

          Vector2 velocity = kinematic->GetVelocity();

          if (velocity.y < 0.f && kinematic->IsColliderMovingAgainstFloor(velocity * deltaTime))
          {
              kinematic->SetVelocity(Vector2::zero);
              kinematic->AdjustPositionToFloor(velocity * deltaTime);
          }
          else if (!kinematic->IsColliderOnFloor())
          {
              if (kinematic->IsColliderMovingAgainstWallX(velocity.x * deltaTime))
                  kinematic->SetVelocityX(-velocity.x);

              kinematic->AddGravity(deltaTime);
          }

          blackboard->accTime -= blackboard->snowballDecPerSecond * deltaTime;

          if (blackboard->accTime <= blackboard->phaseThreshold[3])
              Transition("Struggle");
      });
    enemyStateRolling->RegisterCallback(AIEventState::Tick,
      [=](float deltaTime)
      {
          Vector2 velocity = kinematic->GetVelocity();

          if (velocity.y < 0.f && kinematic->IsColliderMovingAgainstFloor(velocity * deltaTime))
          {
              kinematic->SetVelocityY(0.f);
              kinematic->AdjustPositionToFloor(velocity * deltaTime);
          }
          else if (!kinematic->IsColliderOnFloor())
          {
              kinematic->SetVelocityY(-blackboard->snowballRollingSpeedX);
          }

          float deltaX = velocity.x * deltaTime;

          if (kinematic->IsColliderMovingAgainstWallX(deltaX)
              || kinematic->IsColliderMovingAgainstBoundaryX(deltaX))
          {
              kinematic->SetVelocityX(-velocity.x);
              ++blackboard->hitCount;
          }
      });
    enemyStateDizzy->RegisterCallback(AIEventState::Tick,
      [=](float deltaTime)
      {
          blackboard->accTime += deltaTime;

          if (blackboard->accTime > blackboard->dizzyTime)
          {
              Transition("Walk");
              blackboard->accTime = 0.f;
          }
      });
#pragma endregion RegisterStateCallbackTick

#pragma region RegisterStateCallbackExit
    enemyStateTurn->RegisterCallback(AIEventState::Exit,
      [=](float deltaTime)
      {
          animation->SetFlipX(!animation->GetFlipX());
          blackboard->direction = -blackboard->direction;
      });
    enemyStateJump->RegisterCallback(AIEventState::Exit,
      [=](float deltaTime)
      {
          kinematic->SetVelocity(Vector2::zero);
      });
    enemyStateFall->RegisterCallback(AIEventState::Exit,
      [=](float deltaTime)
      {
          kinematic->SetVelocity(Vector2::zero);
      });
    enemyStateStruggle->RegisterCallback(AIEventState::Exit,
      [=](float deltaTime)
      {
          snowballAnimation->ChangeAnimationClip("snowball_none");
      });
#pragma endregion RegisterStateCallbackExit

#pragma region AIConditions
    auto conditionTurned = CreateAICondition("Turned", ConditionOperator::And,
      [=]() -> bool
      {
          return blackboard->turned;
      });
    auto conditionMoveAgainstBoundaryX
      = CreateAICondition("MoveAgainstBoundaryX", ConditionOperator::And,
        [=]() -> bool
        {
            return kinematic->IsColliderMovingAgainstBoundaryX(blackboard->previousDelta.x);
        });
    auto conditionNotOnFloor       = CreateAICondition("NotOnFloor", ConditionOperator::And,
            [=]() -> bool
            {
          return !kinematic->IsColliderOnFloor();
      });
    auto conditionMoveAgainstWall  = CreateAICondition("MoveAgainstWall", ConditionOperator::And,
       [=]() -> bool
       {
          return kinematic->IsColliderMovingAgainstWallX(blackboard->previousDelta.x);
      });
    auto conditionMoveAgainstFloor = CreateAICondition("MoveAgainstFloor", ConditionOperator::And,
      [=]() -> bool
      {
          bool isFalling = kinematic->GetVelocity().y < 0.f;
          bool wasColliderBottomOnBlock
            = kinematic->IsColliderBottomOnBlock(-blackboard->previousDelta);
          bool isColliderOnFloor = kinematic->IsColliderOnFloor();

          return isFalling && !wasColliderBottomOnBlock && isColliderOnFloor;
      });
    auto conditionIsPlayerAbove    = CreateAICondition("IsPlayerAbove", ConditionOperator::And,
         [=]() -> bool
         {
          Ptr<Player> player          = level->GetPlayer();
          float       playerPositionY = player->GetWorldPosition().y;
          float       thisPositionY   = actor->GetWorldPosition().y;

          return playerPositionY > thisPositionY && playerPositionY - thisPositionY > 0.9f;
      });
    auto conditionHasLandingTileForward
      = CreateAICondition("HasLandingTileForward", ConditionOperator::And,
        [=]() -> bool
        {
            Ptr<Tile> tile           = tilemap->GetTile(actor->GetWorldPosition().ToVector2());
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
        [=]() -> bool
        {
            Ptr<Tile> tile           = tilemap->GetTile(actor->GetWorldPosition().ToVector2());
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
    auto conditionTouchedBlock   = CreateAICompositeCondition("TouchedBlock", ConditionOperator::Or,
        conditionMoveAgainstBoundaryX, conditionMoveAgainstWall);
    auto conditionShouldJumpNext = CreateAICompositeCondition("ShouldJumpNext",
      ConditionOperator::And, conditionMoveAgainstWall, conditionHasLandingTileForward);
    auto conditionCanJumpAbove   = CreateAICompositeCondition(
      "CanJumpAbove", ConditionOperator::And, conditionIsPlayerAbove, conditionHasLandingTileAbove);
    auto conditionShouldJump = CreateAICompositeCondition(
      "ShouldJump", ConditionOperator::Or, conditionShouldJumpNext, conditionCanJumpAbove);
#pragma endregion AIConditions

#pragma region AITransitions
    enemyStateWalk->CreateAITransition("Walk_Fall", enemyStateFall, conditionNotOnFloor);
    enemyStateWalk->CreateAITransition("Walk_Jump", enemyStateJump, conditionShouldJump);
    enemyStateWalk->CreateAITransition("Walk_Turn", enemyStateTurn, conditionTouchedBlock);
    enemyStateJump->CreateAITransition("Jump_Crouch", enemyStateCrouch, conditionMoveAgainstFloor);
    enemyStateFall->CreateAITransition("Fall_Crouch", enemyStateCrouch, conditionMoveAgainstFloor);
    enemyStateTurn->CreateAITransition("Turn_Walk", enemyStateWalk, conditionTurned);
#pragma endregion AITransitions

    _currentState = enemyStateWalk;
}

void GoblinStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}
