#include "pch.h"

#include "SnowbrosEnemy.h"

#include "Core/ResourceManager.h"

#include "GoblinStateMachine.h"
#include "MonkeyStateMachine.h"
#include "SnowballMorphableEnemyBlackboard.h"
#include "SpitterStateMachine.h"
#include "AI/AIComponent.h"
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
    auto palette = FIND_PALETTE("enemy_goblin");
    sprite->SetPaletteNumber(palette->GetID());
    sprite->AttachToComponent(_root);
    sprite->SetRelativeScale(Vector3::one);

    auto snowballSprite = CreateSceneComponent<IndexedSpriteInstanceComponent>("SpriteSnowball");
    snowballSprite->SetEnable(false);
    snowballSprite->SetRenderLayer("Snowball");
    palette = FIND_PALETTE("snowball");
    snowballSprite->SetPaletteNumber(palette->GetID());
    snowballSprite->AttachToComponent(_root);
    snowballSprite->SetRelativeScale(Vector3::one);

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
        aiComponent->CreateAIStateMachine<GoblinStateMachine>();
        break;
    case SnowbrosEnemyType::Monkey:
        aiComponent->CreateAIStateMachine<MonkeyStateMachine>();
        break;
    case SnowbrosEnemyType::Spitter:
        aiComponent->CreateAIStateMachine<SpitterStateMachine>();
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
    auto blackboard   = stateMachine->GetAIBlackboard<SnowballMorphableEnemyBlackboard>();

    sprite->SetFlipX(direction > 0);
    blackboard->direction = direction;
}
