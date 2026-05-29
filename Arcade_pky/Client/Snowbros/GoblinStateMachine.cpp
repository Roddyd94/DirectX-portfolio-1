#include "pch.h"

#include "GoblinStateMachine.h"

#include "Core/Collision/CollisionManager.h"
#include "Core/TimeManager.h"

#include "GoblinBlackboard.h"
#include "SnowProjectile.h"
#include "SnowProjectileComponent.h"
#include "SnowbrosEnemyState.h"
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

    auto sprite    = actor->FindSceneComponent<SpriteComponent>("Sprite");
    auto animation = sprite->CreateAnimation();
    animation->SetAnimationSequence("goblin");
    animation->ChangeAnimationClip("goblin_walk");

    auto spriteBehind    = actor->FindSceneComponent<SpriteComponent>("SpriteBehind");
    auto animationBehind = spriteBehind->CreateAnimation();
    animationBehind->SetAnimationSequence("goblin");
    animationBehind->ChangeAnimationClip("goblin_none");
    spriteBehind->SetEnable(false);

    auto spriteSnowball    = actor->FindSceneComponent<SpriteComponent>("SpriteSnowball");
    auto animationSnowball = spriteSnowball->CreateAnimation();
    animationSnowball->SetAnimationSequence("snowball");
    animationSnowball->ChangeAnimationClip("snowball_none");
    spriteSnowball->SetEnable(false);

#pragma region AIStates
    auto enemyStateStand  = CreateAIState<SnowbrosEnemyState>("Stand", SnowbrosEnemyState::Stand);
    auto enemyStatePatrol = CreateAIState<SnowbrosEnemyState>("Patrol", SnowbrosEnemyState::Patrol);
    auto enemyStateWalk   = CreateAIState<SnowbrosEnemyState>("Walk", SnowbrosEnemyState::Walk);
    enemyStateWalk->SetInterval(0.1f);
    auto enemyStateTurn   = CreateAIState<SnowbrosEnemyState>("Turn", SnowbrosEnemyState::Turn);
    auto enemyStateJump   = CreateAIState<SnowbrosEnemyState>("Jump", SnowbrosEnemyState::Jump);
    auto enemyStateFall   = CreateAIState<SnowbrosEnemyState>("Fall", SnowbrosEnemyState::Fall);
    auto enemyStateCrouch = CreateAIState<SnowbrosEnemyState>("Crouch", SnowbrosEnemyState::Crouch);
    auto enemyStateDizzy  = CreateAIState<SnowbrosEnemyState>("Dizzy", SnowbrosEnemyState::Dizzy);
    auto enemyStateStruggle
      = CreateAIState<SnowbrosEnemyState>("Struggle", SnowbrosEnemyState::Struggle);
    auto enemyStateSnowball
      = CreateAIState<SnowbrosEnemyState>("Snowball", SnowbrosEnemyState::Snowball);
    auto enemyStateRolling
      = CreateAIState<SnowbrosEnemyState>("SnowballRolling", SnowbrosEnemyState::SnowballRolling);
    auto enemyStateCrashing
      = CreateAIState<SnowbrosEnemyState>("SnowballCrashing", SnowbrosEnemyState::SnowballCrashing);
    auto enemyStateLaunched
      = CreateAIState<SnowbrosEnemyState>("Launched", SnowbrosEnemyState::Launched);
    auto enemyStateDead = CreateAIState<SnowbrosEnemyState>("Dead", SnowbrosEnemyState::Dead);
#pragma endregion AIStates

    _currentState = enemyStateWalk;

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
              auto thisState     = Cast<AIState, SnowbrosEnemyState>(_currentState);
              auto thisStateType = thisState->GetStateType();
              if (thisStateType == SnowbrosEnemyState::Snowball)
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

              auto thisState     = Cast<AIState, SnowbrosEnemyState>(_currentState);
              auto thisStateType = thisState->GetStateType();
              switch (thisStateType)
              {
              case SnowbrosEnemyState::Struggle:
                  blackboard->accTime += blackboard->snowballIncForming;
                  break;
              case SnowbrosEnemyState::Snowball:
                  blackboard->accTime += blackboard->snowballIncFormed;
                  break;
              case SnowbrosEnemyState::SnowballRolling:
              case SnowbrosEnemyState::SnowballCrashing:
              case SnowbrosEnemyState::Launched:
              case SnowbrosEnemyState::Dead:
                  break;
              default:
                  Transition("Struggle");
                  blackboard->accTime = blackboard->snowballFormingInitialValue;
                  break;
              }
          }
          break;
          case ColliderType::Enemy:
          {
              auto otherActor        = otherCollider->GetOwner();
              auto otherAI           = otherActor->FindActorComponent<AIComponent>("AI");
              auto otherStateMachine = otherAI->GetAIStateMachine();
              auto otherState        = otherStateMachine->GetCurrentState<SnowbrosEnemyState>();
              auto otherStateType    = otherState->GetStateType();

              auto thisState     = Cast<AIState, SnowbrosEnemyState>(_currentState);
              auto thisStateType = thisState->GetStateType();

              switch (otherStateType)
              {
              case SnowbrosEnemyState::SnowballRolling:
              {
                  switch (thisStateType)
                  {
                  case SnowbrosEnemyState::Snowball:
                      Transition("SnowballRolling");
                      break;
                  case SnowbrosEnemyState::Stand:
                  case SnowbrosEnemyState::Patrol:
                  case SnowbrosEnemyState::Walk:
                  case SnowbrosEnemyState::Turn:
                  case SnowbrosEnemyState::Jump:
                  case SnowbrosEnemyState::Fall:
                  case SnowbrosEnemyState::Crouch:
                  case SnowbrosEnemyState::Dizzy:
                  case SnowbrosEnemyState::Struggle:
                      // todo: transition to Launched
                      break;
                  case SnowbrosEnemyState::SnowballRolling:
                  case SnowbrosEnemyState::SnowballCrashing:
                  case SnowbrosEnemyState::Launched:
                  case SnowbrosEnemyState::Dead:
                      // do nothing
                      break;
                  }
              }
              break;
              case SnowbrosEnemyState::Stand:
              case SnowbrosEnemyState::Patrol:
              case SnowbrosEnemyState::Walk:
              case SnowbrosEnemyState::Turn:
              case SnowbrosEnemyState::Jump:
              case SnowbrosEnemyState::Fall:
              case SnowbrosEnemyState::Crouch:
              case SnowbrosEnemyState::Dizzy:
              case SnowbrosEnemyState::Struggle:
              case SnowbrosEnemyState::Snowball:
              case SnowbrosEnemyState::SnowballCrashing:
              case SnowbrosEnemyState::Launched:
              case SnowbrosEnemyState::Dead:
                  break;
              }
          }
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
              auto otherState        = otherStateMachine->GetCurrentState<SnowbrosEnemyState>();
              auto otherStateType    = otherState->GetStateType();

              auto thisState     = Cast<AIState, SnowbrosEnemyState>(_currentState);
              auto thisStateType = thisState->GetStateType();
              switch (thisStateType)
              {
              case SnowbrosEnemyState::Walk:
              {
                  switch (otherStateType)
                  {
                  case SnowbrosEnemyState::Stand:
                  case SnowbrosEnemyState::Patrol:
                  case SnowbrosEnemyState::Walk:
                  case SnowbrosEnemyState::Turn:
                  case SnowbrosEnemyState::Crouch:
                  case SnowbrosEnemyState::Struggle:
                  case SnowbrosEnemyState::Dizzy:
                  {
                      if (thisPosition.x < otherPosition.x && blackboard->direction < 0)
                          return;

                      if (thisPosition.x > otherPosition.x && blackboard->direction > 0)
                          return;

                      Transition("Turn");
                  }
                  break;
                  }
              }
              break;
              case SnowbrosEnemyState::Snowball:
              {
                  if (otherStateType != SnowbrosEnemyState::Snowball)
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
              break;
              }
          }
          break;
          case ColliderType::EnemyProjectile:
          {
              float deltaTime = TimeManager::Instance().GetDeltaTime();

              auto thisState     = Cast<AIState, SnowbrosEnemyState>(_currentState);
              auto thisStateType = thisState->GetStateType();

              switch (thisStateType)
              {
              case SnowbrosEnemyState::Struggle:
              case SnowbrosEnemyState::Snowball:
              {
                  // todo melt
              }
              break;
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
    animationSnowball->AddNotify("snowball_crash", animationSnowball->GetClipFrameCount("snowball_crash"),
      [=]()
      {
          spriteSnowball->SetEnable(false);
          actor->SetActive(false);
      });
#pragma endregion AnimationNotifies

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
          sprite->SetEnable(false);
          spriteBehind->SetEnable(true);
          spriteSnowball->SetEnable(true);
          animationSnowball->ChangeAnimationClip("snowball_forming", false);
          animationBehind->ChangeAnimationClip("goblin_struggle");
      });
    enemyStateSnowball->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          blackboard->previousPosition = actor->GetWorldPosition().ToVector2();
          blackboard->accTime += blackboard->snowballFormedBonusValue;
          sprite->SetEnable(false);
          spriteSnowball->SetEnable(true);
          animationSnowball->ChangeAnimationClip("snowball_formed", false);
      });
    enemyStateRolling->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          blackboard->hitCount = 0;
          sprite->SetEnable(false);
          spriteSnowball->SetEnable(true);
          animationSnowball->ChangeAnimationClip("snowball_rolling");
      });
    enemyStateDizzy->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_awake");
      });
    enemyStateCrashing->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
            // todo particle
          animationSnowball->ChangeAnimationClip("snowball_crash");
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
              animationSnowball->SetFrameIndex(2);
          else if (blackboard->accTime > blackboard->phaseThreshold[1])
              animationSnowball->SetFrameIndex(1);
          else if (blackboard->accTime > blackboard->phaseThreshold[0])
              animationSnowball->SetFrameIndex(0);
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
              int32 frameIndex = spriteSnowball->GetFrameIndex() + 1;
              int32 frameCount = spriteSnowball->GetFrameCount();
              spriteSnowball->SetFrameIndex(frameIndex % frameCount);
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
              if (kinematic->IsColliderOnFirstFloor())
              {
                  Transition("SnowballCrashing");
                  kinematic->SetVelocity(Vector2::zero);
                  return;
              }

              kinematic->SetVelocityX(-velocity.x);
              ++blackboard->hitCount;
              if (blackboard->hitCount >= 10)
                  Transition("SnowballCrashing");
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
          animationSnowball->ChangeAnimationClip("snowball_none");
          sprite->SetEnable(true);
          spriteSnowball->SetEnable(false);
          spriteBehind->SetEnable(false);
      });
    enemyStateSnowball->RegisterCallback(AIEventState::Exit,
      [=](float deltaTime)
      {
          sprite->SetEnable(true);
          spriteSnowball->SetEnable(false);
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
}

void GoblinStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}
