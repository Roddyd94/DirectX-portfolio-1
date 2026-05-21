#include "pch.h"

#include "GoblinStateMachine.h"

#include "Core/TimeManager.h"

#include "GoblinBlackboard.h"
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

    auto collider          = actor->FindSceneComponent<AABBCollisionComponent>("Collider");
    auto kinematic         = actor->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    auto sprite            = actor->FindSceneComponent<SpriteComponent>("Root");
    auto animation         = sprite->CreateAnimation();
    auto animationSequence = animation->GetSequence();
    // todo 길 찾기 컴포넌트

    animation->SetAnimationSequence("goblin");
    animation->ChangeAnimationClip("goblin_walk");

#pragma region ColliderCallbacks
    collider->RegisterCollisionCallBack(CollisionState::Enter,
      [=](Weak<CollisionComponent> collider)
      {
          Ptr<CollisionComponent> colliderLock = Lock(collider);
          ColliderType::Type      colliderType = colliderLock->GetColliderType();

          switch (colliderType)
          {
          case ColliderType::Enemy:
              if (_currentState->GetName() == "Walk")
                  Transition("Turn");
              break;
          case ColliderType::Snowball:
              break;
          case ColliderType::PlayerProjectile:
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
    animation->AddNotify("goblin_awake", animation->GetClipFrameCount("goblin_crouch"),
      [=]()
      {
          this->Transition("Dizzy");
      });
    animation->AddNotify("goblin_falling", animation->GetClipFrameCount("goblin_crouch"),
      [=]()
      {
          owner->GetOwner()->SetActive(false);
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
    auto enemyStateLaunched = CreateAIState("Launched");
    auto enemyStateDead     = CreateAIState("Dead");
#pragma endregion AIStates

#pragma region RegisterStateCallbackEnter
    enemyStateWalk->RegisterCallback(AIEventState::Enter,
      [=](float deltaTime)
      {
          animation->ChangeAnimationClip("goblin_walk");
          kinematic->MoveX(blackboard->direction * blackboard->walkSpeedX);
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
          bool DidColliderMoveAgainstFloor = kinematic->DidColliderMoveAgainstFloor(blackboard->previousDelta);

          return isFalling && !wasColliderBottomOnBlock && DidColliderMoveAgainstFloor;
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
