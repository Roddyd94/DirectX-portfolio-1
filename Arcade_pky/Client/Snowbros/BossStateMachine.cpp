#include "pch.h"

#include "BossStateMachine.h"

#include "Common/LogManager.h"
#include "Core/ResourceManager.h"
#include "Core/TimeManager.h"

#include "BossBlackboard.h"
#include "BossState.h"
#include "IndexedSpriteInstanceComponent.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosLevel.h"
#include "SpawnBlackboard.h"
#include "AI/AIComponent.h"
#include "AI/AIController.h"
#include "Common/Random.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Collision/PointCollisionComponent.h"
#include "Core/Palette.h"
#include "Core/Pawn.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Tilemap/Tilemap.h"

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

    blackboard->currentPaletteID = FIND_PALETTE("boss_normal")->GetID();

    // blackboard->loopTimerID = TimeManager::Instance().SetTimer(5.0f, true,
    //   [this]()
    //   {
    //       auto pawn       = GetPawn<SnowbrosEnemy>();
    //       auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
    //       auto blackboard = GetBlackboard<BossBlackboard>();

    //      ++blackboard->temp %= 2;
    //      switch (blackboard->temp)
    //      {
    //      case 0:
    //          kinematic->SetVelocity({0.f, -2.f});
    //          break;
    //      case 1:
    //          kinematic->SetVelocity({0.f, 2.f});
    //          break;
    //      }
    //  });

    auto spriteEffectCallback = [this](bool on)
    {
        auto pawn       = GetPawn<SnowbrosEnemy>();
        auto blackboard = GetBlackboard<BossBlackboard>();

        auto sprite      = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
        auto spriteLower = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("SpriteLower");
        auto paletteNumber = FIND_PALETTE("boss_damaged")->GetID();

        if (on && blackboard->damaged)
        {
            sprite->SetPaletteNumber(paletteNumber);
            spriteLower->SetPaletteNumber(paletteNumber);
        }
        else
        {
            sprite->SetPaletteNumber(blackboard->currentPaletteID);
            spriteLower->SetPaletteNumber(blackboard->currentPaletteID);
        }
    };

#pragma region AIStates
    auto enemyStateStand  = CreateAIState<BossState>("Stand", SnowbrosEnemyState::Stand);
    auto enemyStateJump   = CreateAIState<BossState>("Jump", SnowbrosEnemyState::Jump);
    auto enemyStateCrouch = CreateAIState<BossState>("Crouch", SnowbrosEnemyState::Crouch);
    auto enemyStateFire   = CreateAIState<BossState>("Fire", SnowbrosEnemyState::Fire);
    auto enemyStateHop    = CreateAIState<BossState>("Hop", SnowbrosEnemyState::Hop);
    auto enemyStateDead   = CreateAIState<BossState>("Dead", SnowbrosEnemyState::Dead);
#pragma endregion AIStates

#pragma region AnimationNotifies
    animation->AddNotify("boss_upper_shout", animation->GetClipFrameCount("boss_upper_shout") - 1,
      [this]()
      {
          auto blackboard = GetBlackboard<BossBlackboard>();
          --blackboard->actionLeft;
      });
    animation->AddNotify("boss_dead", 1,
      [this]()
      {
          auto pawn = GetPawn<SnowbrosEnemy>();

          auto collider = pawn->FindSceneComponent<AABBCollisionComponent>("Collider");
          auto sprite   = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

          sprite->ChangeAnimation("boss_dead");
          sprite->SetWorldScale({7.f, 3.f});
          sprite->SetRelativePosition({0.f, 2.f});

          collider->SetBoxSize({7.f, 2.5f});
          collider->SetRelativePosition({0.f, 2.f});
      });
#pragma endregion AnimationNotifies

    enemyStateStand->RegisterCallbackSpriteEffect(spriteEffectCallback);
    enemyStateJump->RegisterCallbackSpriteEffect(spriteEffectCallback);
    enemyStateCrouch->RegisterCallbackSpriteEffect(spriteEffectCallback);
    enemyStateFire->RegisterCallbackSpriteEffect(spriteEffectCallback);
    enemyStateHop->RegisterCallbackSpriteEffect(spriteEffectCallback);
    enemyStateDead->RegisterCallbackSpriteEffect(spriteEffectCallback);

#pragma region ColliderCallbacks
    collider->RegisterCollisionCallBack(CollisionState::Enter,
      [this](Weak<CollisionComponent> collider)
      {
          auto otherCollider     = Lock(collider);
          auto otherColliderType = otherCollider->GetColliderType();

          switch (otherColliderType)
          {
          case ColliderType::Enemy:
          {
              auto otherPawn       = Cast<Actor, SnowbrosEnemy>(otherCollider->GetOwner());
              auto otherController = otherPawn->GetController();

              auto otherEnemyType = otherPawn->GetEnemyType();

              auto otherAI           = otherController->FindActorComponent<AIComponent>("AI");
              auto otherStateMachine = otherAI->GetAIStateMachine();

              switch (otherEnemyType)
              {
              case SnowbrosEnemyType::Goblin:
              case SnowbrosEnemyType::Monkey:
              case SnowbrosEnemyType::Spitter:
              case SnowbrosEnemyType::Spawn:
              {
                  auto otherState     = otherStateMachine->GetCurrentState<SnowbrosEnemyState>();
                  auto otherStateType = otherState->GetStateType();

                  if (otherStateType != SnowbrosEnemyState::SnowballRolling)
                      break;

                  Hit(500);
              }
              break;
              }
          }
          break;
          case ColliderType::PlayerProjectile:
          {
              Hit(2);
          }
          break;
          }
      });
#pragma endregion ColliderCallbacks

#pragma region RegisterStateCallbackEnter
    enemyStateStand->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();

          if (pawn->GetWorldPosition().y > blackboard->targetFloorBoundary)
              blackboard->targetFloor = 0;
          else
              blackboard->targetFloor = 1;

          blackboard->jumpTimerID = TimeManager::Instance().SetTimer(1.f, false,
            [this]()
            {
                Jump();
            });
      });
    enemyStateJump->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          auto blackboard = GetBlackboard<BossBlackboard>();

          blackboard->spawnedTop    = false;
          blackboard->spawnedMiddle = false;
          blackboard->spawnedBottom = false;

          int32 randomValue = Utility::RandomInt(0, 10);

          if (randomValue > 7)
              blackboard->spawnPattern = 1;
          else
              blackboard->spawnPattern = 0;
      });
    enemyStateFire->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();
          auto sprite     = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

          sprite->ChangeAnimation("boss_upper_shout");
          blackboard->actionLeft = 4;

          if (pawn->GetWorldPosition().y > blackboard->targetFloorBoundary)
              blackboard->targetFloor = 0;
          else
              blackboard->targetFloor = 1;
      });
    enemyStateHop->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();
          auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");
          auto sprite     = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

          blackboard->actionLeft = 4;

          kinematic->AddForce(blackboard->hopForce);
      });
    enemyStateDead->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();

          auto collider = pawn->FindSceneComponent<AABBCollisionComponent>("Collider");
          auto sprite   = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
          auto spriteLower
            = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("SpriteLower");

          blackboard->targetFloor = 1;
          TimeManager::Instance().RemoveTimer(blackboard->jumpTimerID);

          spriteLower->SetEnable(false);
          sprite->ChangeAnimation("boss_dead");
          sprite->SetWorldScale({6.f, 5.f});
          sprite->SetRelativePosition({0.f, 3.f});

          collider->SetBoxSize({6.f, 4.5f});
          collider->SetRelativePosition({0.f, 3.f});
      });
#pragma endregion RegisterStateCallbackEnter

#pragma region RegisterStateCallbackTick
    enemyStateCrouch->RegisterCallback(AIEventState::Tick,
      [this](float deltaTime)
      {
          LogManager::Instance().Debug("Crouch!", TimeManager::Instance().GetFrameCount());
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();
          auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

          kinematic->SetVelocity(Vector2::zero);
          kinematic->AdjustPositionToFloor();

          int32 pattern = Utility::RandomInt(0, 7);
          if (pattern > 5)
              Transition("Stand");
          else if (pattern > 2)
              Transition("Fire");
          else
              Transition("Hop");
      });
    enemyStateJump->RegisterCallback(AIEventState::Tick,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();
          auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

          kinematic->AddGravity(deltaTime);
          AdjustLowerBody();

          if (0 == blackboard->targetFloor && kinematic->GetVelocity().y > 0.f)
              return;

          float posY = pawn->GetWorldPosition().y;
          if (!blackboard->spawnedTop && std::abs(posY - blackboard->spawnPositionYTop) < 0.1f)
          {
              if (0 == blackboard->targetFloor && 0 == blackboard->spawnPattern)
              {
                  blackboard->spawnedTop = true;
                  return;
              }

              if (0 == blackboard->targetFloor)
                  SpawnSpawn(false);
              else
                  SpawnSpawn(true);

              blackboard->spawnedTop = true;
          }
          else if (!blackboard->spawnedMiddle
                   && std::abs(posY - blackboard->spawnPositionYMiddle) < 0.1f)
          {
              if (0 == blackboard->targetFloor)
                  SpawnSpawn(false);
              else
                  SpawnSpawn(true);

              blackboard->spawnedMiddle = true;
          }
          else if (!blackboard->spawnedBottom
                   && std::abs(posY - blackboard->spawnPositionYBottom) < 0.1f)
          {
              if (0 == blackboard->targetFloor)
                  SpawnSpawn(false);
              else
                  SpawnSpawn(true);

              blackboard->spawnedBottom = true;
          }
      });
    enemyStateFire->RegisterCallback(AIEventState::Tick,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();
          auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

          if (0 == blackboard->actionLeft)
          {
              kinematic->SetVelocity(Vector2::zero);
              Jump();
          }
      });
    enemyStateHop->RegisterCallback(AIEventState::Tick,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();
          auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

          kinematic->AddGravity(deltaTime);

          Vector2 delta = kinematic->GetVelocity() * deltaTime;

          if (kinematic->IsColliderMovingAgainstFloor(delta))
          {
              kinematic->SetVelocity(Vector2::zero);
              kinematic->AdjustPositionToFloor(delta);

              if (0 == blackboard->actionLeft)
              {
                  if (pawn->GetWorldPosition().y > blackboard->targetFloorBoundary)
                      blackboard->targetFloor = 0;
                  else
                      blackboard->targetFloor = 1;

                  Jump();
                  return;
              }

              --blackboard->actionLeft;
              kinematic->AddForce(blackboard->hopForce);
          }

          AdjustLowerBody();
      });
    enemyStateDead->RegisterCallback(AIEventState::Tick,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();
          auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

          bool isColliderOnFloor = kinematic->IsColliderOnFloor();
          if (isColliderOnFloor && kinematic->GetVelocity() == Vector2::zero)
              return;

          kinematic->AddGravity(deltaTime);
          Vector2 delta = kinematic->GetVelocity() * deltaTime;

          bool isFalling                = delta.y < 0.f;
          bool wasColliderBottomOnBlock = kinematic->IsColliderBottomOnBlock(-delta);

          if (isFalling && !wasColliderBottomOnBlock && isColliderOnFloor)
          {
              kinematic->SetVelocity(Vector2::zero);
              kinematic->AdjustPositionToFloor(delta);
          }
      });
#pragma endregion RegisterStateCallbackTick

#pragma region RegisterStateCallbackExit
    enemyStateFire->RegisterCallback(AIEventState::Exit,
      [this](float deltaTime)
      {
          auto pawn       = GetPawn<SnowbrosEnemy>();
          auto blackboard = GetBlackboard<BossBlackboard>();
          auto sprite     = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

          sprite->ChangeAnimation("boss_upper_shut");
      });
#pragma endregion RegisterStateCallbackExit

#pragma region AIConditions
    auto conditionMoveAgainstFloor = CreateAICondition("MoveAgainstBoundaryX",
      ConditionOperator::And,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime) -> bool
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          Vector2 delta = kinematic->GetVelocity() * deltaTime;

          bool isFalling                = delta.y < 0.f;
          bool wasColliderBottomOnBlock = kinematic->IsColliderBottomOnBlock(-delta);
          bool isColliderOnFloor        = kinematic->IsColliderOnFloor();

          return isFalling && !wasColliderBottomOnBlock && isColliderOnFloor;
      });
    auto conditionIsAtProperFloor  = CreateAICondition("IsAtProperFloor", ConditionOperator::And,
       [this](float deltaTime) -> bool
       {
          auto    pawn       = GetPawn();
          auto    blackboard = GetBlackboard<BossBlackboard>();
          Vector2 position   = pawn->GetWorldPosition().ToVector2();

          bool canLandOnFloor1
            = blackboard->targetFloor == 0 && position.y < blackboard->targetFloorBoundary;
          bool canLandOnFloor2
            = blackboard->targetFloor == 1 && position.y > blackboard->targetFloorBoundary;

          return canLandOnFloor1 || canLandOnFloor2;
      });
    auto conditionCanLand          = CreateAICompositeCondition(
      "CanLand", ConditionOperator::And, conditionMoveAgainstFloor, conditionIsAtProperFloor);
#pragma endregion AIConditions

#pragma region AITransitions
    enemyStateJump->CreateAITransition("Jump_Crouch", enemyStateCrouch, conditionCanLand);
#pragma endregion AITransitions

    Transition("Stand");

    return true;
}

void BossStateMachine::Destroy()
{
    auto blackboard = GetBlackboard<BossBlackboard>();
    TimeManager::Instance().RemoveTimer(blackboard->damageTimerID);
    TimeManager::Instance().RemoveTimer(blackboard->jumpTimerID);

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
        collider->SetBoxSize({3.5f, 4.5f});
        collider->SetRelativePosition({0.75f, 2.5f});
        spriteUpper->ChangeAnimation("boss_upper_shut");
        spriteLower->SetRelativeScale({5.f, 5.f});
        spriteLower->SetRelativePosition({0.f, 2.5f});
        spriteLower->ChangeAnimation("boss_lower_low");
    }
    break;
    case SnowbrosBossLowerFrameType::Mid:
    {
        collider->SetBoxSize({3.5f, 5.5f});
        collider->SetRelativePosition({0.75f, 2.f});
        spriteUpper->ChangeAnimation("boss_upper_normal");
        spriteLower->SetRelativeScale({5.f, 6.f});
        spriteLower->SetRelativePosition({0.f, 2.f});
        spriteLower->ChangeAnimation("boss_lower_mid");
    }
    break;
    case SnowbrosBossLowerFrameType::High:
    {
        collider->SetBoxSize({3.5f, 6.5f});
        collider->SetRelativePosition({0.75f, 1.5f});
        spriteUpper->ChangeAnimation("boss_upper_open");
        spriteLower->SetRelativeScale({5.f, 7.f});
        spriteLower->SetRelativePosition({0.f, 1.5f});
        spriteLower->ChangeAnimation("boss_lower_high");
    }
    break;
    }
}

void BossStateMachine::Hit(int32 damage)
{
    auto pawn       = GetPawn<SnowbrosEnemy>();
    auto blackboard = GetBlackboard<BossBlackboard>();

    auto sprite = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");

    blackboard->hp -= damage;
    blackboard->damaged = true;

    TimeManager::Instance().RemoveTimer(blackboard->damageTimerID);

    blackboard->damageTimerID = TimeManager::Instance().SetTimer(1.f, false,
      [this]()
      {
          auto blackboard     = GetBlackboard<BossBlackboard>();
          blackboard->damaged = false;
      });

    if (blackboard->hp > 0)
        return;

    auto currentState = GetCurrentState();
    if (currentState && currentState->GetName() == "Dead")
        return;

    blackboard->currentPaletteID = FIND_PALETTE("boss_dead")->GetID();
    Transition("Dead");
}

void BossStateMachine::Jump()
{
    auto pawn       = GetPawn<SnowbrosEnemy>();
    auto blackboard = GetBlackboard<BossBlackboard>();
    auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    float positionX       = pawn->GetWorldPosition().x;
    float targetPositionX = Utility::RandomInt(6, 11) / 2.f;

    Vector2 jumpForce = blackboard->GetTargetJumpForceX(targetPositionX - positionX);
    kinematic->AddForce(jumpForce);

    LogManager::Instance().Debug("Jumped!", TimeManager::Instance().GetFrameCount());
    Transition("Jump");
}

void BossStateMachine::SpawnSpawn(bool spawnOnLeft)
{
    auto blackboard = GetBlackboard<BossBlackboard>();

    auto pawn  = GetPawn();
    auto level = pawn->GetLevel();

    float spawnPositionX = 0.f;
    float positionError  = Utility::RandomInt(-10, 11) / 10.f;

    Vector3 position = pawn->GetWorldPosition();
    Vector3 scale    = Vector3::one * 2.f;
    Vector3 rotation = Vector3::zero;

    Vector3 spawnPosition = position;
    spawnPosition.x += -1.f;
    spawnPosition.y += 3.f;

    auto spawn = level->SpawnActor<SnowbrosEnemy>(spawnPosition, scale, rotation);
    spawn->SetEnemyType(SnowbrosEnemyType::Spawn);

    auto spawnAI         = spawn->GetAIComponent();
    auto spawnBlackboard = spawnAI->GetBlackboard<SpawnBlackboard>();

    if (spawnOnLeft)
    {
        spawn->SetDirection(1.f);
        spawnBlackboard->landPositionX = blackboard->spawnLandPositionXLeft + positionError;
    }
    else
    {
        spawn->SetDirection(-1.f);
        if (1 == blackboard->spawnPattern
            && std::abs(position.y - blackboard->spawnPositionYTop) < 0.1f)
            spawnBlackboard->landPositionX
              = blackboard->spawnLandPositionXRight + blackboard->spawnPattern1Bias + positionError;
        else
            spawnBlackboard->landPositionX = blackboard->spawnLandPositionXRight + positionError;
    }
}

void BossStateMachine::AdjustLowerBody()
{
    auto pawn       = GetPawn<SnowbrosEnemy>();
    auto blackboard = GetBlackboard<BossBlackboard>();
    auto kinematic  = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    auto level   = Cast<Level, SnowbrosLevel>(pawn->GetLevel());
    auto tilemap = level->GetTilemap();

    auto pointHigh = pawn->FindSceneComponent<PointCollisionComponent>("ColliderLegHigh");
    auto pointMid  = pawn->FindSceneComponent<PointCollisionComponent>("ColliderLegMid");
    auto pointLow  = pawn->FindSceneComponent<PointCollisionComponent>("ColliderLegLow");

    auto tileHigh = tilemap->GetTile(pointHigh->GetWorldPosition().ToVector2());
    auto tileMid  = tilemap->GetTile(pointMid->GetWorldPosition().ToVector2());
    auto tileLow  = tilemap->GetTile(pointLow->GetWorldPosition().ToVector2());

    Vector2 position = pawn->GetWorldPosition().ToVector2();
    Vector2 velocity = kinematic->GetVelocity();

    bool pointTouchedTopBlockHigh = tileHigh && tileHigh->IsTopBlock();
    bool pointTouchedTopBlockMid  = tileMid && tileMid->IsTopBlock();
    bool pointTouchedTopBlockLow  = tileLow && tileLow->IsTopBlock();

    if (blackboard->targetFloor == 0.f && velocity.y < 0.f
        && position.y > blackboard->targetFloorBoundary)
        ChangeLowerBody(SnowbrosBossLowerFrameType::High);
    else if (blackboard->targetFloor == 1 && velocity.y > 0.f
             && position.y > blackboard->targetFloorBoundary)
        return;
    else
    {
        if (!pointTouchedTopBlockHigh && !pointTouchedTopBlockMid && !pointTouchedTopBlockLow)
            ChangeLowerBody(SnowbrosBossLowerFrameType::High);
        else if (pointTouchedTopBlockLow)
            ChangeLowerBody(SnowbrosBossLowerFrameType::Mid);
        else
            ChangeLowerBody(SnowbrosBossLowerFrameType::Low);
    }
}
