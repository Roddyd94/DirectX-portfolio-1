#include "pch.h"

#include "GoblinStateMachine.h"

#include "Core/Collision/CollisionManager.h"
#include "Core/TimeManager.h"

#include "GoblinBlackboard.h"
#include "Item.h"
#include "SnowProjectile.h"
#include "SnowProjectileComponent.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "SnowbrosLevel.h"
#include "AI/AIComponent.h"
#include "Common/Random.h"
#include "Core/Actor.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Animation/SpriteInstanceComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Player/Player.h"
#include "Tilemap/Tilemap.h"

void GoblinStateMachine::Init(Ptr<class AIComponent> owner)
{
    AIStateMachine::Init(owner);
    auto blackboard = CreateBlackboard<GoblinBlackboard>();

    auto pawn    = owner->GetPawn<SnowbrosEnemy>();
    auto level   = Cast<Level, SnowbrosLevel>(owner->GetLevel());
    auto tilemap = level->GetTilemap();

    auto collider  = pawn->FindSceneComponent<AABBCollisionComponent>("Collider");
    auto kinematic = pawn->FindActorComponent<PlatformerKinematicComponent>("Kinematic");

    auto sprite    = pawn->FindSceneComponent<SpriteInstanceComponent>("Sprite");
    auto animation = sprite->CreateAnimation();
    animation->SetAnimationSequence("goblin");
    animation->ChangeAnimationClip("goblin_walk");

    auto spriteSnowball    = pawn->FindSceneComponent<SpriteInstanceComponent>("SpriteSnowball");
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

              auto otherPawn  = Cast<Actor, Pawn>(otherCollider->GetOwner());
              auto otherActor = otherPawn->GetController();

              auto otherAI           = otherActor->FindActorComponent<AIComponent>("AI");
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
                  case SnowbrosEnemyState::Crouch:
                  case SnowbrosEnemyState::Dizzy:
                  case SnowbrosEnemyState::Struggle:
                  {
                      auto otherBlackboard
                        = otherStateMachine->GetAIBlackboard<SnowballMorphableEnemyBlackboard>();

                      otherBlackboard->direction = thisPosition.x < otherPosition.x ? 1.f : -1.f;
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
      [weakBlackboard = Weak(blackboard)]()
      {
          auto blackboard    = Lock(weakBlackboard);
          blackboard->turned = true;
      });
    animation->AddNotify("goblin_crouch", animation->GetClipFrameCount("goblin_crouch"),
      [this]()
      {
          Transition("Walk");
      });
    animation->AddNotify("goblin_jump", animation->GetClipFrameCount("goblin_jump"),
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)]()
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          kinematic->AddForce(blackboard->GetTargetJumpForce());
          blackboard->isJumping = true;
      });
    animation->AddNotify("goblin_awake", animation->GetClipFrameCount("goblin_awake"),
      [weakAnimation = Weak(animation)]()
      {
          auto animation = Lock(weakAnimation);
          animation->ChangeAnimationClip("goblin_dizzy");
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
    animation->AddNotify("goblin_dead", animation->GetClipFrameCount("goblin_dead"),
      [weakLevel = Weak(level), weakBlackboard = Weak(blackboard), weakPawn = Weak(pawn)]()
      {
          auto pawn       = Lock(weakPawn);
          auto level      = Lock(weakLevel);
          auto blackboard = Lock(weakBlackboard);

          auto item
            = level->SpawnActor<Item>(pawn->GetWorldPosition(), Vector3::one, Vector3::zero);
          int32 randomValue = Utility::RandomInt(0, 100);
          if (blackboard->hitByReinforced)
          {
              if (randomValue >= 80)
                  item->SetItemType(Item::Speed);
              else if (randomValue >= 60)
                  item->SetItemType(Item::Power);
              else if (randomValue >= 40)
                  item->SetItemType(Item::Range);
              else
              {
                  item->SetItemType(Item::Sushi);

                  int32 itemNumber = randomValue % 5;
                  item->SetItemNumber(itemNumber + 6);
              }
          }
          else
          {
              if (randomValue >= 80)
                  item->SetItemType(Item::Speed);
              else if (randomValue >= 60)
                  item->SetItemType(Item::Power);
              else if (randomValue >= 40)
                  item->SetItemType(Item::Range);
              else
              {
                  item->SetItemType(Item::Sushi);

                  int32 itemNumber = randomValue % 5;
                  item->SetItemNumber(itemNumber + 1);
              }
          }

          pawn->SetActive(false);
      });
#pragma endregion AnimationNotifies

#pragma region RegisterStateCallbackEnter
    enemyStateWalk->RegisterCallback(AIEventState::Enter,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard),
        weakAnimation = Weak(animation)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto animation  = Lock(weakAnimation);
          auto blackboard = Lock(weakBlackboard);

          animation->ChangeAnimationClip("goblin_walk");
          kinematic->SetVelocityX(blackboard->direction * blackboard->walkSpeedX);
      });
    enemyStateTurn->RegisterCallback(AIEventState::Enter,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard),
        weakAnimation = Weak(animation)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto animation  = Lock(weakAnimation);
          auto blackboard = Lock(weakBlackboard);

          blackboard->turned = false;
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_turn");
      });
    enemyStateJump->RegisterCallback(AIEventState::Enter,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard),
        weakAnimation = Weak(animation)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto animation  = Lock(weakAnimation);
          auto blackboard = Lock(weakBlackboard);

          blackboard->isJumping = false;
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_jump");
      });
    enemyStateFall->RegisterCallback(AIEventState::Enter,
      [weakKinematic = Weak(kinematic), weakAnimation = Weak(animation)](float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);
          auto animation = Lock(weakAnimation);

          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_midair");
      });
    enemyStateCrouch->RegisterCallback(AIEventState::Enter,
      [weakKinematic = Weak(kinematic), weakAnimation = Weak(animation)](float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);
          auto animation = Lock(weakAnimation);

          kinematic->AdjustPositionToFloor();
          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_crouch");
      });
    enemyStateStruggle->RegisterCallback(AIEventState::Enter,
      [weakKinematic = Weak(kinematic), weakSprite = Weak(sprite),
        weakSpriteSnowball = Weak(spriteSnowball)](float deltaTime)
      {
          auto kinematic      = Lock(weakKinematic);
          auto sprite         = Lock(weakSprite);
          auto spriteSnowball = Lock(weakSpriteSnowball);

          kinematic->SetVelocity(Vector2::zero);
          sprite->SetRenderLayer("EnemyBehind");
          sprite->ChangeAnimation("goblin_struggle");
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
              spriteSnowball->ChangeAnimation("snowball_reinforced");
          else
              spriteSnowball->ChangeAnimation("snowball_rolling");
      });
    enemyStateDizzy->RegisterCallback(AIEventState::Enter,
      [weakKinematic = Weak(kinematic), weakAnimation = Weak(animation)](float deltaTime)
      {
          auto kinematic = Lock(weakKinematic);
          auto animation = Lock(weakAnimation);

          kinematic->SetVelocity(Vector2::zero);
          animation->ChangeAnimationClip("goblin_awake");
      });
    enemyStateCrashing->RegisterCallback(AIEventState::Enter,
      [weakAnimationSnowball = Weak(animationSnowball)](float deltaTime)
      {
          // todo particle,

          auto animationSnowball = Lock(weakAnimationSnowball);
          animationSnowball->ChangeAnimationClip("snowball_crash");
      });
    enemyStateLaunched->RegisterCallback(AIEventState::Enter,
      [weakBlackboard = Weak(blackboard), weakKinematic = Weak(kinematic),
        weakSprite = Weak(sprite), levelBoundaryTop = level->GetBoundary().top](float deltaTime)
      {
          auto blackboard = Lock(weakBlackboard);
          auto sprite     = Lock(weakSprite);
          auto kinematic  = Lock(weakKinematic);
          kinematic->SetVelocity(Vector2::zero);

          blackboard->accTime = 0;

          float   repulsionY = std::min(levelBoundaryTop - sprite->GetWorldPosition().y, 5.f);
          Vector2 repulsion
            = SnowballMorphableEnemyBlackboard::CalculateForceToTarget({8.f, repulsionY});
          kinematic->AddForce(repulsion); // todo

          sprite->ChangeAnimation("goblin_launched");
          TimeManager::Instance().SetTimer(0.3f, false,
            [sprite]()
            {
                sprite->ChangeAnimation("goblin_airborne");
            });
      });
    enemyStateDead->RegisterCallback(AIEventState::Enter,
      [weakSprite = Weak(sprite)](float deltaTime)
      {
          auto sprite = Lock(weakSprite);
          sprite->ChangeAnimation("goblin_dead");
      });
#pragma endregion RegisterStateCallbackEnter

#pragma region RegisterStateCallbackTick
    enemyStateWalk->RegisterCallback(AIEventState::Tick,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          kinematic->SetVelocity({blackboard->direction * blackboard->walkSpeedX, 0.f});
          blackboard->previousDelta = kinematic->GetVelocity() * deltaTime;
      });
    enemyStateJump->RegisterCallback(AIEventState::Tick,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          if (blackboard->isJumping)
              kinematic->AddGravity(deltaTime);
          blackboard->previousDelta = kinematic->GetVelocity() * deltaTime;
      });
    enemyStateFall->RegisterCallback(AIEventState::Tick,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)](float deltaTime)
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          kinematic->AddGravity(deltaTime);
          blackboard->previousDelta = kinematic->GetVelocity() * deltaTime;
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
    auto conditionTurned = CreateAICondition("Turned", ConditionOperator::And,
      [weakBlackboard = Weak(blackboard)]() -> bool
      {
          auto blackboard = Lock(weakBlackboard);
          return blackboard->turned;
      });
    auto conditionMoveAgainstBoundaryX
      = CreateAICondition("MoveAgainstBoundaryX", ConditionOperator::And,
        [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)]() -> bool
        {
            auto kinematic  = Lock(weakKinematic);
            auto blackboard = Lock(weakBlackboard);

            return kinematic->IsColliderMovingAgainstBoundaryX(blackboard->previousDelta.x);
        });
    auto conditionNotOnFloor       = CreateAICondition("NotOnFloor", ConditionOperator::And,
            [weakKinematic = Weak(kinematic)]() -> bool
            {
          auto kinematic = Lock(weakKinematic);
          return !kinematic->IsColliderOnFloor();
      });
    auto conditionMoveAgainstWall  = CreateAICondition("MoveAgainstWall", ConditionOperator::And,
       [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)]() -> bool
       {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          return kinematic->IsColliderMovingAgainstWallX(blackboard->previousDelta.x);
      });
    auto conditionMoveAgainstFloor = CreateAICondition("MoveAgainstFloor", ConditionOperator::And,
      [weakKinematic = Weak(kinematic), weakBlackboard = Weak(blackboard)]() -> bool
      {
          auto kinematic  = Lock(weakKinematic);
          auto blackboard = Lock(weakBlackboard);

          bool isFalling = kinematic->GetVelocity().y < 0.f;
          bool wasColliderBottomOnBlock
            = kinematic->IsColliderBottomOnBlock(-blackboard->previousDelta);
          bool isColliderOnFloor = kinematic->IsColliderOnFloor();

          return isFalling && !wasColliderBottomOnBlock && isColliderOnFloor;
      });
    auto conditionIsPlayerAbove    = CreateAICondition("IsPlayerAbove", ConditionOperator::And,
         [weakLevel = Weak(level), weakPawn = Weak(pawn)]() -> bool
         {
          auto level = Lock(weakLevel);
          auto pawn  = Lock(weakPawn);

          Ptr<Player> player          = level->GetPlayer();
          float       playerPositionY = player->GetWorldPosition().y;
          float       thisPositionY   = pawn->GetWorldPosition().y;

          return playerPositionY > thisPositionY && playerPositionY - thisPositionY > 0.9f;
      });
    auto conditionHasLandingTileForward
      = CreateAICondition("HasLandingTileForward", ConditionOperator::And,
        [weakTilemap = Weak(tilemap), weakBlackboard = Weak(blackboard),
          weakPawn = Weak(pawn)]() -> bool
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
        [weakTilemap = Weak(tilemap), weakBlackboard = Weak(blackboard),
          weakPawn = Weak(pawn)]() -> bool
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
