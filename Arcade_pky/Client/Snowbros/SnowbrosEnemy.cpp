#include "pch.h"

#include "SnowbrosEnemy.h"

#include "Core/ResourceManager.h"

#include "GoblinBlackboard.h"
#include "GoblinStateMachine.h"
#include "MonkeyBlackboard.h"
#include "MonkeyStateMachine.h"
#include "SnowballMorphableEnemyBlackboard.h"
#include "SpitterBlackboard.h"
#include "SpitterStateMachine.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Palette.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Snowbros/IndexedSpriteInstanceComponent.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool SnowbrosEnemy::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    AI::Init(id, position, scale, rotation);

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto sprite = CreateSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    sprite->SetRenderLayer("Enemy");
    sprite->AttachToComponent(_root);
    sprite->SetRelativeScale(Vector3::one);

    auto spriteSnowball = CreateSceneComponent<IndexedSpriteInstanceComponent>("SpriteSnowball");
    spriteSnowball->SetRenderLayer("Snowball");
    spriteSnowball->SetEnable(false);

    auto animationSnowball = spriteSnowball->CreateAnimation();
    animationSnowball->SetAnimationSequence("snowball");
    animationSnowball->ChangeAnimationClip("snowball_none");

    auto palette = FIND_PALETTE("snowball");
    spriteSnowball->SetPaletteNumber(palette->GetID());
    spriteSnowball->AttachToComponent(_root);
    spriteSnowball->SetRelativeScale(Vector3::one);

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(_root);
    collider->SetBoxSize({0.9f, 1.2f});
    collider->SetCollisionProfile("Enemy");

    auto kinematic = CreateActorComponent<PlatformerKinematicComponent>("Kinematic");
    kinematic->SetCollider(collider);
    kinematic->SetTilemap(tilemap);

    return true;
}

void SnowbrosEnemy::SetEnemyType(SnowbrosEnemyType enemyType)
{
    auto aiComponent = GetAIComponent();
    _enemyType       = enemyType;

    switch (_enemyType)
    {
    case SnowbrosEnemyType::Goblin:
    {
        auto sprite    = FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
        auto animation = sprite->CreateAnimation();
        animation->SetAnimationSequence("goblin");
        auto palette = FIND_PALETTE("enemy_goblin");
        sprite->SetPaletteNumber(palette->GetID());

        aiComponent->CreateAIStateMachine<GoblinStateMachine>();
    }
    break;
    case SnowbrosEnemyType::Monkey:
    {
        auto sprite    = FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
        auto animation = sprite->CreateAnimation();
        animation->SetAnimationSequence("monkey");
        auto palette = FIND_PALETTE("enemy_monkey");
        sprite->SetPaletteNumber(palette->GetID());

        aiComponent->CreateAIStateMachine<MonkeyStateMachine>();
    }
    break;
    case SnowbrosEnemyType::Spitter:
    {
        auto sprite    = FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
        auto animation = sprite->CreateAnimation();
        animation->SetAnimationSequence("spitter");
        auto palette = FIND_PALETTE("enemy_spitter");
        sprite->SetPaletteNumber(palette->GetID());

        aiComponent->CreateAIStateMachine<SpitterStateMachine>();
    }
    break;
    case SnowbrosEnemyType::Pumpkin:
    case SnowbrosEnemyType::Ghost:
        break;
    default:
        break;
    }
}

void SnowbrosEnemy::SetDirection(float direction)
{
    auto aiComponent = GetAIComponent();

    auto sprite       = FindComponent<SpriteInstanceComponent>("Sprite");
    auto stateMachine = aiComponent->GetAIStateMachine();
    auto blackboard   = stateMachine->GetBlackboard<SnowballMorphableEnemyBlackboard>();

    sprite->SetFlipX(direction > 0);
    blackboard->direction = direction;
}
