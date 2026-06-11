#include "pch.h"

#include "SnowballMorphableEnemyStateMachine.h"

#include "Core/Collision/CollisionManager.h"
#include "Core/ResourceManager.h"
#include "Core/TimeManager.h"

#include "IndexedSpriteInstanceComponent.h"
#include "Item.h"
#include "SnowProjectile.h"
#include "SnowProjectileComponent.h"
#include "SnowballMorphableEnemyBlackboard.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "SnowbrosLevel.h"
#include "AI/AIComponent.h"
#include "AI/AIController.h"
#include "Common/Random.h"
#include "Core/Actor.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Palette.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Player/Player.h"
#include "Tilemap/Tilemap.h"

bool SnowballMorphableEnemyStateMachine::TryMoveX(float deltaX)
{
    auto pawn      = GetPawn();
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    if (kinematic->IsColliderMovingAgainstBoundaryX(deltaX))
        return false;

    if (kinematic->IsColliderMovingAgainstWallX(deltaX))
        return false;

    auto thisCollider = pawn->FindSceneComponent<AABBCollisionComponent>("Collider");

    Rect    thisRect     = thisCollider->GetBox();
    Vector2 thisPosition = thisCollider->GetWorldPosition().ToVector2();

    auto level          = Cast<Level, SnowbrosLevel>(pawn->GetLevel());
    auto player         = level->GetPlayer();
    auto playerCollider = player->FindSceneComponent<AABBCollisionComponent>("Collider");
    Rect playerRect     = playerCollider->GetBox();

    if (deltaX > 0.f && thisRect.right > playerRect.left && thisRect.right < playerRect.right
        && thisPosition.y < playerRect.top && thisPosition.y > playerRect.bottom)
        return false;

    if (deltaX < 0.f && thisRect.left < playerRect.right && thisRect.left > playerRect.left
        && thisPosition.y < playerRect.top && thisPosition.y > playerRect.bottom)
        return false;

    auto                                      collisionManager = level->GetCollisionManager();
    std::vector<Weak<AABBCollisionComponent>> snowballs;
    SnowballMorphableEnemyStateMachine::FindSnowballs(collisionManager, snowballs);

    for (auto& snowball : snowballs)
    {
        auto otherCollider = Lock(snowball);

        if (thisCollider->GetColliderID() == otherCollider->GetColliderID())
            continue;

        Rect    otherRect     = otherCollider->GetBox();
        Vector2 otherPosition = otherCollider->GetWorldPosition().ToVector2();

        if (otherRect.top < thisRect.bottom)
            continue;

        if (thisRect.top < otherRect.bottom)
            continue;

        if (otherRect.right < thisRect.left)
            continue;

        if (thisRect.right < otherRect.left)
            continue;

        bool canMoveLeft  = thisPosition.x > otherPosition.x && deltaX < 0.f;
        bool canMoveRight = thisPosition.x < otherPosition.x && deltaX > 0.f;

        if (!canMoveLeft && !canMoveRight)
            continue;

        auto otherPawn  = Cast<Actor, Pawn>(otherCollider->GetOwner());
        auto otherActor = otherPawn->GetController();

        auto otherAI = otherActor->FindActorComponent<AIComponent>("AI");
        auto otherStateMachine
          = Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(otherAI->GetAIStateMachine());

        if (nullptr == otherStateMachine)
            continue;

        if (otherStateMachine->TryMoveX(deltaX))
        {
            kinematic->MoveX(deltaX);
            return true;
        }
        else
            return false;
    }

    kinematic->MoveX(deltaX);
    return true;
}

void SnowballMorphableEnemyStateMachine::Throw(float direction)
{
    auto pawn      = GetPawn();
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    auto blackboard = Cast<AIBlackboard, SnowballMorphableEnemyBlackboard>(_blackboard);

    kinematic->SetVelocityX(direction * blackboard->snowballRollingSpeedX);
    Transition("SnowballRolling");
}

bool SnowballMorphableEnemyStateMachine::Init(Ptr<class AIComponent> owner)
{
    AIStateMachine::Init(owner);

    auto blackboard = GetBlackboard<SnowballMorphableEnemyBlackboard>();

    auto pawn    = owner->GetPawn<SnowbrosEnemy>();
    auto level   = Cast<Level, SnowbrosLevel>(owner->GetLevel());
    auto tilemap = level->GetTilemap();

    auto collider  = pawn->FindSceneComponent<AABBCollisionComponent>("Collider");
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    auto sprite    = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();

    auto spriteSnowball
      = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("SpriteSnowball");
    auto animationSnowball = spriteSnowball->GetAnimation();

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

    _currentState = FindAIState<SnowbrosEnemyState>("Walk");

#pragma region AnimationNotifies
    AddNotifyToAnimationClipEnd(SnowbrosEnemyAnimationType::Turn,
      [weakBlackboard = Weak(blackboard)]()
      {
          auto blackboard    = Lock(weakBlackboard);
          blackboard->turned = true;
      });
    AddNotifyToAnimationClipEnd(SnowbrosEnemyAnimationType::Crouch,
      [this]()
      {
          Transition("Walk");
      });
    AddNotifyToAnimationClipEnd(SnowbrosEnemyAnimationType::Jump,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)]()
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          kinematic->AddForce(blackboard->GetTargetJumpForce());
          blackboard->isJumping = true;
      });
    AddNotifyToAnimationClipEnd(SnowbrosEnemyAnimationType::Awake,
      [this]()
      {
          ChangeAnimationClip(SnowbrosEnemyAnimationType::Dizzy);
      });
    AddNotifyToAnimationClipEnd(SnowbrosEnemyAnimationType::Dead,
      [weakLevel = Weak(level), weakBlackboard = Weak(blackboard), weakPawn = Weak(pawn)]()
      {
          auto pawn       = Lock(weakPawn);
          auto level      = Lock(weakLevel);
          auto blackboard = Lock(weakBlackboard);

          Vector3 spawnPosition = pawn->GetWorldPosition();

          int32 randomValue = Utility::RandomInt(0, 100);
          if (blackboard->hitByReinforced)
          {
              if (randomValue >= 80)
                  level->SpawnItem(spawnPosition, Item::Speed);
              else if (randomValue >= 60)
                  level->SpawnItem(spawnPosition, Item::Power);
              else if (randomValue >= 40)
                  level->SpawnItem(spawnPosition, Item::Range);
              else
              {
                  auto item = level->SpawnItem(spawnPosition, Item::Sushi);

                  int32 itemNumber = randomValue % 5;
                  item->SetItemNumber(itemNumber + 6);
              }
          }
          else
          {
              if (randomValue >= 80)
                  level->SpawnItem(spawnPosition, Item::Speed);
              else if (randomValue >= 60)
                  level->SpawnItem(spawnPosition, Item::Power);
              else if (randomValue >= 40)
                  level->SpawnItem(spawnPosition, Item::Range);
              else
              {
                  auto item = level->SpawnItem(spawnPosition, Item::Sushi);

                  int32 itemNumber = randomValue % 5;
                  item->SetItemNumber(itemNumber + 1);
              }
          }

          pawn->SetActive(false);
      });

    animationSnowball->AddNotify("snowball_crash",
      animationSnowball->GetClipFrameCount("snowball_crash"),
      [weakPawn = Weak(pawn), weakSpriteSnowball = Weak(spriteSnowball)]()
      {
          auto pawn           = Lock(weakPawn);
          auto spriteSnowball = Lock(weakSpriteSnowball);

          pawn->SetActive(false);
          spriteSnowball->SetEnable(false);
      });
#pragma endregion AnimationNotifies

#pragma region ColliderCallbacks
    collider->RegisterCollisionCallBack(CollisionState::Enter,
      [this, weakCollider = Weak(collider), weakKinematic = Weak(kinematic),
        weakBlackboard = Weak(blackboard)](Weak<CollisionComponent> collider)
      {
          auto thisCollider = Lock(weakCollider);
          auto kinematic    = Lock(weakKinematic);
          auto blackboard   = Lock(weakBlackboard);

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

              float formingMultiplier = projectile->IsPoweredUp() ? 2.f : 1.f;

              auto thisState     = Cast<AIState, SnowbrosEnemyState>(_currentState);
              auto thisStateType = thisState->GetStateType();
              switch (thisStateType)
              {
              case SnowbrosEnemyState::Struggle:
                  blackboard->accTime += blackboard->snowballIncForming * formingMultiplier;
                  break;
              case SnowbrosEnemyState::Snowball:
                  blackboard->accTime += blackboard->snowballIncFormed * formingMultiplier;
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
          }
      });
    collider->RegisterCollisionCallBack(CollisionState::Stay,
      [this, weakPawn = Weak(pawn), weakCollider = Weak(collider), weakKinematic = Weak(kinematic),
        weakBlackboard = Weak(blackboard)](Weak<CollisionComponent> collider)
      {
          auto pawn         = Lock(weakPawn);
          auto thisCollider = Lock(weakCollider);
          auto kinematic    = Lock(weakKinematic);
          auto blackboard   = Lock(weakBlackboard);

          Ptr<CollisionComponent> otherCollider     = Lock(collider);
          ColliderType::Type      otherColliderType = otherCollider->GetColliderType();

          switch (otherColliderType)
          {
          case ColliderType::Enemy:
          {
              auto thisPosition  = pawn->GetWorldPosition();
              auto otherPosition = otherCollider->GetWorldPosition();

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
                      case SnowbrosEnemyState::Fire:
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
                      switch (otherStateType)
                      {
                      case SnowbrosEnemyState::Snowball:
                      {
                          auto otherSnowballStateMachine
                            = Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(
                              otherStateMachine);

                          float distance = std::abs(thisPosition.x - otherPosition.x);
                          if (distance > blackboard->snowballRepulsiveDistance)
                              return;

                          if (thisPosition.x < otherPosition.x)
                          {
                              otherSnowballStateMachine->TryMoveX(
                                blackboard->snowballRepulsiveDeltaX);
                              TryMoveX(-blackboard->snowballRepulsiveDeltaX);
                          }
                          else
                          {
                              otherSnowballStateMachine->TryMoveX(
                                -blackboard->snowballRepulsiveDeltaX);
                              TryMoveX(blackboard->snowballRepulsiveDeltaX);
                          }
                      }
                      break;
                      case SnowbrosEnemyState::SnowballRolling:
                      {
                          auto thisPosition  = thisCollider->GetWorldPosition().ToVector2();
                          auto otherPosition = otherCollider->GetWorldPosition().ToVector2();

                          if (thisPosition.x < otherPosition.x)
                              kinematic->SetVelocity({-blackboard->snowballRollingSpeedX, 0.f});
                          else
                              kinematic->SetVelocity({blackboard->snowballRollingSpeedX, 0.f});

                          blackboard->isSnowballReinforced = true;
                          Transition("SnowballRolling");

                          {
                              auto otherKinematic
                                = otherPawn->FindActorComponent<PlatformerKinematicComponent>(
                                  "Kinematic");

                              Vector2 otherVelocity = otherKinematic->GetVelocity();
                              otherKinematic->SetVelocityX(-otherVelocity.x);
                          }
                      }
                      break;
                      }
                  }
                  break;
                  case SnowbrosEnemyState::SnowballRolling:
                  {
                      switch (otherStateType)
                      {
                      case SnowbrosEnemyState::Stand:
                      case SnowbrosEnemyState::Patrol:
                      case SnowbrosEnemyState::Walk:
                      case SnowbrosEnemyState::Turn:
                      case SnowbrosEnemyState::Jump:
                      case SnowbrosEnemyState::Fall:
                      case SnowbrosEnemyState::Fire:
                      case SnowbrosEnemyState::Crouch:
                      case SnowbrosEnemyState::Dizzy:
                      case SnowbrosEnemyState::Struggle:
                      {
                          auto otherBlackboard
                            = otherStateMachine->GetBlackboard<SnowballMorphableEnemyBlackboard>();

                          otherBlackboard->direction
                            = thisPosition.x < otherPosition.x ? 1.f : -1.f;
                          otherBlackboard->hitByReinforced = blackboard->isSnowballReinforced;

                          otherStateMachine->Transition("Launched");
                      }
                      break;
                      }
                  }
                  break;
                  }
              }
              break;
              case SnowbrosEnemyType::Pumpkin:
                  break;
              case SnowbrosEnemyType::Ghost:
                  break;
              case SnowbrosEnemyType::Boss:
              {
                  auto thisState     = Cast<AIState, SnowbrosEnemyState>(_currentState);
                  auto thisStateType = thisState->GetStateType();

                  if (SnowbrosEnemyState::SnowballRolling == thisStateType)
                      Transition("SnowballCrashing");
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
                  auto otherPawn = otherCollider->GetOwner();
                  auto otherKinematic = otherPawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

                  otherKinematic->SetVelocityX(0.f);

                  Transition("Struggle");
                  blackboard->accTime = 0.f;
              }
              break;
              }
          }
          break;
          }
      });
#pragma endregion ColliderCallbacks

#pragma region RegisterStateCallbackEnter
    enemyStateWalk->RegisterCallback(AIEventState::Enter,
      [this, weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);
          kinematic->SetVelocityX(blackboard->direction * blackboard->walkSpeedX);
          ChangeAnimationClip(SnowbrosEnemyAnimationType::Walk);
      });
    enemyStateTurn->RegisterCallback(AIEventState::Enter,
      [this, weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          blackboard->turned = false;
          kinematic->SetVelocity(Vector2::zero);
          ChangeAnimationClip(SnowbrosEnemyAnimationType::Turn);
      });
    enemyStateJump->RegisterCallback(AIEventState::Enter,
      [this, weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          blackboard->isJumping = false;
          kinematic->SetVelocity(Vector2::zero);
          ChangeAnimationClip(SnowbrosEnemyAnimationType::Jump);
      });
    enemyStateFall->RegisterCallback(AIEventState::Enter,
      [this, weakKinematic = Weak(kinematic)](float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);

          kinematic->SetVelocity(Vector2::zero);
          ChangeAnimationClip(SnowbrosEnemyAnimationType::Midair);
      });
    enemyStateCrouch->RegisterCallback(AIEventState::Enter,
      [this, weakKinematic = Weak(kinematic)](float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);

          kinematic->AdjustPositionToFloor();
          kinematic->SetVelocity(Vector2::zero);

          ChangeAnimationClip(SnowbrosEnemyAnimationType::Crouch);
      });
    enemyStateStruggle->RegisterCallback(AIEventState::Enter,
      [this, weakKinematic = Weak(kinematic), weakSprite = Weak(sprite),
        weakSpriteSnowball = Weak(spriteSnowball)](float deltaTime)
      {
          auto kinematic      = Lock(weakKinematic);
          auto sprite         = Lock(weakSprite);
          auto spriteSnowball = Lock(weakSpriteSnowball);

          kinematic->SetVelocity(Vector2::zero);
          sprite->SetRenderLayer("EnemyBehind");
          ChangeAnimationClip(SnowbrosEnemyAnimationType::Struggle);
          spriteSnowball->SetEnable(true);
          spriteSnowball->ChangeAnimation("snowball_forming", false);
      });
    enemyStateSnowball->RegisterCallback(AIEventState::Enter,
      [weakBlackboard = Weak(blackboard), weakPawn = Weak(pawn), weakSprite = Weak(sprite),
        weakSpriteSnowball = Weak(spriteSnowball)](float deltaTime)
      {
          auto blackboard     = Lock(weakBlackboard);
          auto pawn           = Lock(weakPawn);
          auto sprite         = Lock(weakSprite);
          auto spriteSnowball = Lock(weakSpriteSnowball);

          blackboard->previousPosition = pawn->GetWorldPosition().ToVector2();
          blackboard->accTime += blackboard->snowballFormedBonusValue;
          sprite->SetEnable(false);
          spriteSnowball->SetEnable(true);
          spriteSnowball->ChangeAnimation("snowball_formed", false);
      });
    enemyStateRolling->RegisterCallback(AIEventState::Enter,
      [weakBlackboard = Weak(blackboard), weakSprite = Weak(sprite),
        weakSpriteSnowball = Weak(spriteSnowball)](float deltaTime)
      {
          auto blackboard     = Lock(weakBlackboard);
          auto sprite         = Lock(weakSprite);
          auto spriteSnowball = Lock(weakSpriteSnowball);

          blackboard->hitCount = 0;
          sprite->SetEnable(false);
          spriteSnowball->SetEnable(true);
          if (blackboard->isSnowballReinforced)
          {
              spriteSnowball->ChangeAnimation("snowball_reinforced");
              auto palette = FIND_PALETTE("player_1");
              spriteSnowball->SetPaletteNumber(palette->GetID());
          }
          else
              spriteSnowball->ChangeAnimation("snowball_rolling");
      });
    enemyStateDizzy->RegisterCallback(AIEventState::Enter,
      [this, weakKinematic = Weak(kinematic)](float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);

          kinematic->SetVelocity(Vector2::zero);
          ChangeAnimationClip(SnowbrosEnemyAnimationType::Awake);
      });
    enemyStateCrashing->RegisterCallback(AIEventState::Enter,
      [weakAnimationSnowball = Weak(animationSnowball), weakKinematic = Weak(kinematic)](
        float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);
          // todo particle,
          kinematic->SetVelocity(Vector2::zero);

          auto animationSnowball = Lock(weakAnimationSnowball);
          animationSnowball->ChangeAnimationClip("snowball_crash");
      });
    enemyStateLaunched->RegisterCallback(AIEventState::Enter,
      [this, weakBlackboard = Weak(blackboard), weakKinematic = Weak(kinematic),
        levelBoundaryTop = level->GetBoundary().top](float deltaTime)
      {
          auto blackboard = Lock(weakBlackboard);
          auto kinematic  = Lock(weakKinematic);
          kinematic->SetVelocity(Vector2::zero);

          blackboard->accTime = 0;

          auto    pawn       = this->GetPawn();
          float   repulsionY = std::min(levelBoundaryTop - pawn->GetWorldPosition().y, 5.f);
          Vector2 repulsion
            = SnowballMorphableEnemyBlackboard::CalculateForceToTarget({8.f, repulsionY});
          kinematic->AddForce(repulsion); // todo

          ChangeAnimationClip(SnowbrosEnemyAnimationType::Launched);
          TimeManager::Instance().SetTimer(0.3f, false,
            [this]()
            {
                ChangeAnimationClip(SnowbrosEnemyAnimationType::Airborne);
            });
      });
    enemyStateDead->RegisterCallback(AIEventState::Enter,
      [this](float deltaTime)
      {
          ChangeAnimationClip(SnowbrosEnemyAnimationType::Dead);
      });
#pragma endregion RegisterStateCallbackEnter

#pragma region RegisterStateCallbackTick
    enemyStateWalk->RegisterCallback(AIEventState::Tick,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          kinematic->SetVelocity({blackboard->direction * blackboard->walkSpeedX, 0.f});
      });
    enemyStateJump->RegisterCallback(AIEventState::Tick,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          if (blackboard->isJumping)
              kinematic->AddGravity(deltaTime);
      });
    enemyStateFall->RegisterCallback(AIEventState::Tick,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          kinematic->AddGravity(deltaTime);
      });
    enemyStateStruggle->RegisterCallback(AIEventState::Tick,
      [this, weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard),
        weakAnimationSnowball = Weak(animationSnowball)](float deltaTime)
      {
          auto kinematic         = Lock(weakKinematic);
          auto blackboard        = Lock(weakBlackboard);
          auto animationSnowball = Lock(weakAnimationSnowball);

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
      [this, weakPawn = Weak(pawn), weakKinematic = Weak(kinematic),
        weakBlackboard     = Weak(blackboard),
        weakSpriteSnowball = Weak(spriteSnowball)](float deltaTime)
      {
          auto pawn           = Lock(weakPawn);
          auto kinematic      = Lock(weakKinematic);
          auto blackboard     = Lock(weakBlackboard);
          auto spriteSnowball = Lock(weakSpriteSnowball);

          Vector2 currentPosition = pawn->GetWorldPosition().ToVector2();
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
      [this, weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

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
      [this, weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto blackboard = Lock(weakBlackboard);

          blackboard->accTime += deltaTime;

          if (blackboard->accTime > blackboard->dizzyTime)
          {
              Transition("Walk");
              blackboard->accTime = 0.f;
          }
      });
    enemyStateLaunched->RegisterCallback(AIEventState::Tick,
      [this, weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          blackboard->accTime += deltaTime;
          Vector2 velocity = kinematic->GetVelocity();
          float   deltaX   = velocity.x * deltaTime;

          kinematic->AddGravity(deltaTime);

          if (velocity.y < 0.f && kinematic->IsColliderMovingAgainstFloor(velocity * deltaTime))
          {
              if (blackboard->accTime > blackboard->airborneTime
                  || kinematic->IsColliderOnFirstFloor())
              {
                  kinematic->SetVelocity(Vector2::zero);
                  Transition("Dead");
              }
          }

          if (kinematic->IsColliderMovingAgainstWallX(deltaX)
              || kinematic->IsColliderMovingAgainstBoundaryX(deltaX))
              kinematic->SetVelocityX(-velocity.x);
      });
#pragma endregion RegisterStateCallbackTick

#pragma region RegisterStateCallbackExit
    enemyStateTurn->RegisterCallback(AIEventState::Exit,
      [weakAnimation = Weak(animation), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto animation  = Lock(weakAnimation);
          auto blackboard = Lock(weakBlackboard);

          animation->SetFlipX(!animation->GetFlipX());
          blackboard->direction = -blackboard->direction;
      });
    enemyStateJump->RegisterCallback(AIEventState::Exit,
      [weakKinematic = Weak(kinematic)](float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);
          kinematic->SetVelocity(Vector2::zero);
      });
    enemyStateFall->RegisterCallback(AIEventState::Exit,
      [weakKinematic = Weak(kinematic)](float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);
          kinematic->SetVelocity(Vector2::zero);
      });
    enemyStateStruggle->RegisterCallback(AIEventState::Exit,
      [weakSprite = Weak(sprite), weakSpriteSnowball = Weak(spriteSnowball)](float deltaTime)
      {
          auto sprite         = Lock(weakSprite);
          auto spriteSnowball = Lock(weakSpriteSnowball);

          spriteSnowball->SetEnable(false);
          spriteSnowball->ChangeAnimation("snowball_none");
          sprite->SetRenderLayer("Enemy");
      });
    enemyStateSnowball->RegisterCallback(AIEventState::Exit,
      [weakSprite = Weak(sprite), weakSpriteSnowball = Weak(spriteSnowball)](float deltaTime)
      {
          auto sprite         = Lock(weakSprite);
          auto spriteSnowball = Lock(weakSpriteSnowball);

          sprite->SetEnable(true);
          spriteSnowball->SetEnable(false);
      });
#pragma endregion RegisterStateCallbackExit

#pragma region AIConditions
    auto conditionMoveAgainstBoundaryX = CreateAICondition("MoveAgainstBoundaryX",
      ConditionOperator::And,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime) -> bool
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          float deltaX = kinematic->GetVelocity().x * deltaTime;

          return kinematic->IsColliderMovingAgainstBoundaryX(deltaX);
      });
    auto conditionNotOnFloor           = CreateAICondition("NotOnFloor", ConditionOperator::And,
                [weakKinematic = Weak(kinematic)](float deltaTime) -> bool
                {
          auto kinematic = Lock(weakKinematic);
          return !kinematic->IsColliderOnFloor();
      });
    auto conditionMoveAgainstWall  = CreateAICondition("MoveAgainstWall", ConditionOperator::And,
       [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime) -> bool
       {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          float deltaX = kinematic->GetVelocity().x * deltaTime;

          return kinematic->IsColliderMovingAgainstWallX(deltaX);
      });
    auto conditionMoveAgainstFloor = CreateAICondition("MoveAgainstFloor", ConditionOperator::And,
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

#pragma endregion AIConditions

#pragma region AITransitions
    enemyStateWalk->CreateAITransition("Walk_Fall", enemyStateFall, conditionNotOnFloor);
    enemyStateJump->CreateAITransition("Jump_Crouch", enemyStateCrouch, conditionMoveAgainstFloor);
    enemyStateFall->CreateAITransition("Fall_Crouch", enemyStateCrouch, conditionMoveAgainstFloor);
#pragma endregion AITransitions

    return true;
}

void SnowballMorphableEnemyStateMachine::FindSnowballs(Ptr<class CollisionManager> collisionManager,
  std::vector<Weak<class AABBCollisionComponent>>&                                 snowballs)
{
    std::vector<Weak<CollisionComponent>> enemyColliders;
    collisionManager->FindColliders(ColliderType::Enemy, enemyColliders);

    for (auto& enemyCollider : enemyColliders)
    {
        auto otherCollider = Lock(enemyCollider);
        if (nullptr == otherCollider)
            continue;

        auto otherPawn = Cast<Actor, Pawn>(otherCollider->GetOwner());
        if (nullptr == otherPawn)
            continue;

        auto otherActor = otherPawn->GetController();
        if (nullptr == otherActor)
            continue;

        auto otherAI = otherActor->FindActorComponent<AIComponent>("AI");
        if (nullptr == otherAI)
            continue;

        auto otherStateMachine = otherAI->GetAIStateMachine();
        if (nullptr == Cast<AIStateMachine, SnowballMorphableEnemyStateMachine>(otherStateMachine))
            continue;

        auto otherState = otherStateMachine->GetCurrentState<SnowbrosEnemyState>();
        if (nullptr == otherState)
            continue;

        if (otherState->GetStateType() != SnowbrosEnemyState::Snowball)
            continue;

        auto aabb = Cast<CollisionComponent, AABBCollisionComponent>(otherCollider);
        snowballs.push_back(aabb);
    }
}
