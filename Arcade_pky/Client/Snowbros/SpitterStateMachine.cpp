#include "pch.h"

#include "SpitterStateMachine.h"

#include "Core/TimeManager.h"

#include "EnemyProjectile.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "SpitterBlackboard.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Level.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Snowbros/IndexedSpriteInstanceComponent.h"

bool SpitterStateMachine::Init(Ptr<class AIComponent> owner)
{
    auto blackboard = CreateBlackboard<SpitterBlackboard>();
    SnowballMorphableEnemyStateMachine::Init(owner);

    auto pawn      = owner->GetPawn<SnowbrosEnemy>();
    auto sprite    = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();
    animation->ChangeAnimationClip("spitter_walk");

    auto enemyStateShoot = CreateAIState<SnowbrosEnemyState>("Shoot");

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

          TimeManager::Instance().SetTimer(1.f, false,
            [this]()
            {
                Transition("Walk");
            });
      });

    auto enemyStateWalk = FindAIState<SnowbrosEnemyState>("Walk");
    auto enemyStateTurn = FindAIState<SnowbrosEnemyState>("Turn");

    auto conditionTurned = CreateAICondition("Turned", ConditionOperator::And,
      [weakBlackboard = Weak(blackboard)]() -> bool
      {
          auto blackboard = Lock(weakBlackboard);
          return blackboard->turned;
      });
    enemyStateTurn->CreateAITransition("Turn_Shoot", enemyStateShoot, conditionTurned);

    return true;
}

void SpitterStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}

void SpitterStateMachine::ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play)
{
    auto owner  = GetOwner();
    auto pawn   = owner->GetPawn<SnowbrosEnemy>();
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
    auto owner     = GetOwner();
    auto pawn      = owner->GetPawn<SnowbrosEnemy>();
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
    default:
        break;
    }
}
