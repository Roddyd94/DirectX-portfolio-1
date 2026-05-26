#include "pch.h"

#include "SnowbrosEnemy.h"

#include "GoblinStateMachine.h"
#include "MonkeyStateMachine.h"
#include "SpitterStateMachine.h"
#include "AI/AIComponent.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/AABBCollisionComponent.h"
#include "Platformer/PlatformerKinematicComponent.h"
#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapLevel.h"

bool SnowbrosEnemy::Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation)
{
    AI::Init(id, position, scale, rotation);

    Ptr<TilemapLevel> level   = Cast<Level, TilemapLevel>(GetLevel());
    Ptr<Tilemap>      tilemap = level->GetTilemap();

    auto sprite = CreateSceneComponent<SpriteComponent>("Sprite");
    sprite->SetRenderLayer("Enemy");
    sprite->SetShader("SpriteShader");
    sprite->AttachToComponent(_root);
    sprite->SetRelativePosition({0.f, 0.0625f, 0.f});
    sprite->SetRelativeScale(Vector3::one);

    auto snowballSprite = CreateSceneComponent<SpriteComponent>("Snowball");
    snowballSprite->SetEnable(false);
    snowballSprite->SetRenderLayer("Snowball");
    snowballSprite->SetShader("SpriteShader");
    snowballSprite->AttachToComponent(_root);
    snowballSprite->SetRelativePosition({0.f, 0.0625f, 0.f});
    snowballSprite->SetRelativeScale(Vector3::one);

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(_root);
    collider->SetBoxSize({0.9f, 1.f});
    collider->SetCollisionProfile("Enemy");

    auto kinematic = CreateActorComponent<PlatformerKinematicComponent>("Kinematic");
    kinematic->SetCollider(collider);
    kinematic->SetTilemap(tilemap);

    return true;
}

void SnowbrosEnemy::SetEnemyType(SnowbrosEnemyType enemyType)
{
    _enemyType = enemyType;

    switch (_enemyType)
    {
    case SnowbrosEnemyType::Goblin:
        _aiComponent->CreateAIStateMachine<GoblinStateMachine>();
        break;
    case SnowbrosEnemyType::Monkey:
        _aiComponent->CreateAIStateMachine<MonkeyStateMachine>();
        break;
    case SnowbrosEnemyType::Spitter:
        _aiComponent->CreateAIStateMachine<SpitterStateMachine>();
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
    auto sprite     = FindComponent<SpriteComponent>("Sprite");
    auto blackboard = _aiComponent->GetAIStateMachine()->GetAIBlackboard<AIBlackboard>();

    sprite->SetFlipX(direction > 0);
    blackboard->SetDirection(direction);
}
