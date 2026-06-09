#include "pch.h"

#include "GoblinStateMachine.h"

#include "GoblinBlackboard.h"
#include "SnowbrosEnemy.h"
#include "SnowbrosEnemyState.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Snowbros/IndexedSpriteInstanceComponent.h"

bool GoblinStateMachine::Init(Ptr<class AIComponent> owner)
{
    auto blackboard = CreateBlackboard<GoblinBlackboard>();
    SnowballMorphableEnemyStateMachine::Init(owner);

    auto pawn      = owner->GetPawn<SnowbrosEnemy>();
    auto sprite    = pawn->FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    auto animation = sprite->GetAnimation();
    animation->ChangeAnimationClip("goblin_walk");

    auto enemyStateWalk = FindAIState<SnowbrosEnemyState>("Walk");
    auto enemyStateTurn = FindAIState<SnowbrosEnemyState>("Turn");

    auto conditionTurned = CreateAICondition("Turned", ConditionOperator::And,
      [weakBlackboard = Weak(blackboard)]() -> bool
      {
          auto blackboard = Lock(weakBlackboard);
          return blackboard->turned;
      });
    enemyStateTurn->CreateAITransition("Turn_Walk", enemyStateWalk, conditionTurned);

    return true;
}

void GoblinStateMachine::Destroy()
{
    AIStateMachine::Destroy();
}

void GoblinStateMachine::ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play)
{
    auto owner  = GetOwner();
    auto pawn   = owner->GetPawn<SnowbrosEnemy>();
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
    auto owner     = GetOwner();
    auto pawn      = owner->GetPawn<SnowbrosEnemy>();
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
