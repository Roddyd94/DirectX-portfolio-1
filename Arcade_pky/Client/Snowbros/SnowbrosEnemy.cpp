#include "pch.h"

#include "SnowbrosEnemy.h"

#include "Core/ResourceManager.h"

#include "BossStateMachine.h"
#include "GoblinBlackboard.h"
#include "GoblinStateMachine.h"
#include "MonkeyBlackboard.h"
#include "MonkeyStateMachine.h"
#include "SnowballMorphableEnemyBlackboard.h"
#include "SpawnStateMachine.h"
#include "SpitterBlackboard.h"
#include "SpitterStateMachine.h"
#include "AI/AIComponent.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Core/Collision/PointCollisionComponent.h"
#include "Core/Palette.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Snowbros/IndexedSpriteInstanceComponent.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool SnowbrosEnemy::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    AI::Init(id, position, scale, rotation);
    SetName("Enemy");

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto sprite = CreateSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
    sprite->SetRenderLayer("Enemy");
    sprite->AttachToComponent(_root);
    sprite->SetRelativeScale(Vector3::one);

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(_root);
    collider->SetBoxSize({0.9f, 1.2f});
    collider->SetCollisionProfile("Enemy");

    auto kinematic = CreateActorComponent<PlatformerKinematicComponent>("Kinematic");
    kinematic->SetCollider(collider);

    return true;
}

SnowbrosEnemyType SnowbrosEnemy::GetEnemyType() const
{
    return _enemyType;
}

void SnowbrosEnemy::SetEnemyType(SnowbrosEnemyType enemyType)
{
    auto aiComponent = GetAIComponent();
    _enemyType       = enemyType;

    switch (_enemyType)
    {
    case SnowbrosEnemyType::Goblin:
    {
        CreateSnowball();
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
        CreateSnowball();
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
        CreateSnowball();
        auto sprite    = FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
        auto animation = sprite->CreateAnimation();
        animation->SetAnimationSequence("spitter");
        auto palette = FIND_PALETTE("enemy_spitter");
        sprite->SetPaletteNumber(palette->GetID());

        aiComponent->CreateAIStateMachine<SpitterStateMachine>();
    }
    break;
    case SnowbrosEnemyType::Spawn:
    {
        CreateSnowball();
        auto sprite    = FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
        auto animation = sprite->CreateAnimation();
        animation->SetAnimationSequence("boss_spawn");
        auto palette = FIND_PALETTE("boss_spawn");
        sprite->SetPaletteNumber(palette->GetID());

        aiComponent->CreateAIStateMachine<SpawnStateMachine>();
    }
    break;
    case SnowbrosEnemyType::Boss:
    {
        SetWorldScale(Vector2::one);

        auto sprite = FindSceneComponent<IndexedSpriteInstanceComponent>("Sprite");
        sprite->SetRelativePosition({0.f, 3.5f});
        sprite->SetRelativeScale({3.f, 3.f});
        auto palette = FIND_PALETTE("boss_normal");
        sprite->SetPaletteNumber(palette->GetID());
        auto animation = sprite->CreateAnimation();
        animation->SetAnimationSequence("boss");
        animation->ChangeAnimationClip("boss_upper_shout");

        auto spriteLower = CreateSceneComponent<IndexedSpriteInstanceComponent>("SpriteLower");
        spriteLower->SetRenderLayer("Enemy");
        spriteLower->AttachToComponent(_root);
        spriteLower->SetPaletteNumber(palette->GetID());

        animation = spriteLower->CreateAnimation();
        animation->SetAnimationSequence("boss");

        auto stateMachine = aiComponent->CreateAIStateMachine<BossStateMachine>();
        stateMachine->ChangeLowerBody(SnowbrosBossLowerFrameType::Low);

        auto dotCollider = CreateSceneComponent<PointCollisionComponent>("ColliderLegLow");
        dotCollider->AttachToComponent(_root);
        dotCollider->SetRelativePosition({0.5f, -0.75f});

        dotCollider = CreateSceneComponent<PointCollisionComponent>("ColliderLegLower");
        dotCollider->AttachToComponent(_root);
        dotCollider->SetRelativePosition({0.5f, -1.75f});
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

    auto sprite = FindComponent<SpriteInstanceComponent>("Sprite");
    sprite->SetFlipX(direction > 0);

    auto stateMachine = aiComponent->GetAIStateMachine();

    switch (_enemyType)
    {
    case SnowbrosEnemyType::Goblin:
    case SnowbrosEnemyType::Monkey:
    case SnowbrosEnemyType::Spitter:
    case SnowbrosEnemyType::Spawn:
    {
        auto blackboard       = stateMachine->GetBlackboard<SnowballMorphableEnemyBlackboard>();
        blackboard->direction = direction;
    }
    break;
    case SnowbrosEnemyType::Pumpkin:
        break;
    case SnowbrosEnemyType::Ghost:
        break;
    case SnowbrosEnemyType::Boss:
        break;
    case SnowbrosEnemyType::End:
        break;
    default:
        break;
    }
}

void SnowbrosEnemy::CreateSnowball()
{
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
}
