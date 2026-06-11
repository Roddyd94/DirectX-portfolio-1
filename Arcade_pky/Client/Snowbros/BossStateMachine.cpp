#include "pch.h"

#include "BossStateMachine.h"

#include "Core/TimeManager.h"

#include "BossBlackboard.h"
#include "IndexedSpriteInstanceComponent.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "SnowbrosLevel.h"
#include "AI/AIComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Pawn.h"
#include "Platformer/PlatformerKinematicComponent.h"

bool BossStateMachine::Init(Ptr<class AIComponent> owner)
{
    AIStateMachine::Init(owner);
    auto blackboard = CreateBlackboard<BossBlackboard>();

    auto pawn    = owner->GetPawn<SnowbrosEnemy>();
    auto level   = Cast<Level, SnowbrosLevel>(owner->GetLevel());
    auto tilemap = level->GetTilemap();

    auto collider  = pawn->FindSceneComponent<AABBCollisionComponent>("Collider");
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    auto sprite    = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();

    blackboard->loopTimerID = TimeManager::Instance().SetTimer(1.0f, true,
      [this]()
      {
          auto blackboard = GetBlackboard<BossBlackboard>();
          ++blackboard->temp;
          blackboard->temp %= 3;
          switch (blackboard->temp)
          {
          case 0:
              ChangeLowerBody(SnowbrosBossLowerFrameType::Low);
              break;
          case 1:
              ChangeLowerBody(SnowbrosBossLowerFrameType::Mid);
              break;
          case 2:
              ChangeLowerBody(SnowbrosBossLowerFrameType::High);
              break;
          }
      });

#pragma region AIStates
    auto enemyStateStand = CreateAIState<SnowbrosEnemyState>("Stand", SnowbrosEnemyState::Stand);
    auto enemyStateJump  = CreateAIState<SnowbrosEnemyState>("Jump", SnowbrosEnemyState::Jump);
    auto enemyStateHop   = CreateAIState<SnowbrosEnemyState>("Hop", SnowbrosEnemyState::Hop);
    auto enemyStateFire  = CreateAIState<SnowbrosEnemyState>("Fire", SnowbrosEnemyState::Fire);
    auto enemyStateDead  = CreateAIState<SnowbrosEnemyState>("Dead", SnowbrosEnemyState::Dead);
#pragma endregion AIStates

#pragma region AnimationNotifies

#pragma endregion AnimationNotifies

#pragma region ColliderCallbacks
#pragma endregion ColliderCallbacks

#pragma region RegisterStateCallbackEnter
#pragma endregion RegisterStateCallbackEnter

#pragma region RegisterStateCallbackTick
    enemyStateJump->RegisterCallback(AIEventState::Tick,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          if (blackboard->isJumping)
              kinematic->AddGravity(deltaTime);
          blackboard->previousDelta = kinematic->GetVelocity() * deltaTime;
      });
#pragma endregion RegisterStateCallbackTick

#pragma region RegisterStateCallbackExit
#pragma endregion RegisterStateCallbackExit

#pragma region AIConditions
    auto conditionMoveAgainstFloor
      = CreateAICondition("MoveAgainstBoundaryX", ConditionOperator::And,
        [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime) -> bool
        {
            auto kinematic  = Lock(weakKinematic);
            auto blackboard = Lock(weakBlackboard);

            bool isFalling = kinematic->GetVelocity().y < 0.f;
            bool wasColliderBottomOnBlock
              = kinematic->IsColliderBottomOnBlock(-blackboard->previousDelta);
            bool isColliderOnFloor = kinematic->IsColliderOnFloor();

            return isFalling && !wasColliderBottomOnBlock && isColliderOnFloor;
        });
#pragma endregion AIConditions

#pragma region AITransitions
#pragma endregion AITransitions

    return true;
}

void BossStateMachine::Destroy()
{
    auto blackboard = GetBlackboard<BossBlackboard>();
    TimeManager::Instance().RemoveTimer(blackboard->loopTimerID);

    AIStateMachine::Destroy();
}

void BossStateMachine::ChangeLowerBody(SnowbrosBossLowerFrameType type)
{
    auto pawn     = GetPawn();
    auto collider = pawn->FindSceneComponent<AABBCollisionComponent>("Collider");

    auto spriteUpper = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto spriteLower = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("SpriteLower");

    switch (type)
    {
    case SnowbrosBossLowerFrameType::Low:
    {
        collider->SetBoxSize({4.f, 4.5f});
        collider->SetRelativePosition({1.f, 2.5f});
        spriteUpper->ChangeAnimation("boss_upper_shut");
        spriteLower->SetRelativeScale({5.f, 5.f});
        spriteLower->SetRelativePosition({0.f, 2.5f});
        spriteLower->ChangeAnimation("boss_lower_low");
    }
    break;
    case SnowbrosBossLowerFrameType::Mid:
    {
        collider->SetBoxSize({4.f, 5.5f});
        collider->SetRelativePosition({1.f, 2.f});
        spriteUpper->ChangeAnimation("boss_upper_normal");
        spriteLower->SetRelativeScale({5.f, 6.f});
        spriteLower->SetRelativePosition({0.f, 2.f});
        spriteLower->ChangeAnimation("boss_lower_mid");
    }
    break;
    case SnowbrosBossLowerFrameType::High:
    {
        collider->SetBoxSize({4.f, 6.5f});
        collider->SetRelativePosition({1.f, 1.5f});
        spriteUpper->ChangeAnimation("boss_upper_open");
        spriteLower->SetRelativeScale({5.f, 7.f});
        spriteLower->SetRelativePosition({0.f, 1.5f});
        spriteLower->ChangeAnimation("boss_lower_high");
    }
    break;
    }
}
