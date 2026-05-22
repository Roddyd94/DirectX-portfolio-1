#include "pch.h"

#include "SnowbrosEnemy.h"

#include "GoblinStateMachine.h"
#include "MonkeyStateMachine.h"
#include "Snowball.h"
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

    auto rootComp = CreateSceneComponent<SpriteComponent>("Root");
    rootComp->SetRenderLayer("Enemy");
    rootComp->SetShader("SpriteShader");
    SetRoot(rootComp);

    auto collider = CreateSceneComponent<AABBCollisionComponent>("Collider");
    collider->AttachToComponent(rootComp);
    collider->SetBoxSize({0.9f, 1.0f});
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
    auto sprite     = Cast<SceneComponent, SpriteComponent>(_root);
    auto blackboard = _aiComponent->GetAIStateMachine()->GetAIBlackboard<AIBlackboard>();

    sprite->SetFlipX(direction > 0);
    blackboard->SetDirection(direction);
}
