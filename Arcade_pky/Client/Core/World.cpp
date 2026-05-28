#include "pch.h"

#include "World.h"

// #include "Level.h"
#include "Snowbros/SnowbrosLevel.h"

#ifdef _HAS_COLLISION_MODULE
#include "Core/Collision/CollisionProfileManager.h"
#endif // _HAS_COLLISION_MODULE

void World::Init(const std::string& name)
{
#ifdef _HAS_COLLISION_MODULE
    _collisionProfileManager = New<CollisionProfileManager>();
    _collisionProfileManager->Init();

    _collisionProfileManager->CreateChannel(ColliderType::Player, CollisionResponse::Ignore);
    _collisionProfileManager->CreateChannel(ColliderType::PlayerHead, CollisionResponse::Ignore);
    _collisionProfileManager->CreateChannel(ColliderType::Enemy, CollisionResponse::Ignore);
    _collisionProfileManager->CreateChannel(
      ColliderType::PlayerProjectile, CollisionResponse::Ignore);
    _collisionProfileManager->CreateChannel(ColliderType::Item, CollisionResponse::Ignore);

    _collisionProfileManager->CreateProfile("Player", ColliderType::Player);
    _collisionProfileManager->CreateProfile("PlayerHead", ColliderType::PlayerHead);
    _collisionProfileManager->CreateProfile("Enemy", ColliderType::Enemy);
    _collisionProfileManager->CreateProfile("PlayerProjectile", ColliderType::PlayerProjectile);
    _collisionProfileManager->CreateProfile("EnemyProjectile", ColliderType::EnemyProjectile);
    _collisionProfileManager->CreateProfile("Item", ColliderType::Item);

    _collisionProfileManager->SetProfileResponse("Player",
      std::make_pair(ColliderType::Enemy, CollisionResponse::Block),
      std::make_pair(ColliderType::EnemyProjectile, CollisionResponse::Block),
      std::make_pair(ColliderType::Item, CollisionResponse::Block));

    _collisionProfileManager->SetProfileResponse(
      "PlayerHead", std::make_pair(ColliderType::Enemy, CollisionResponse::Block));

    _collisionProfileManager->SetProfileResponse("Enemy",
      std::make_pair(ColliderType::Player, CollisionResponse::Block),
      std::make_pair(ColliderType::PlayerHead, CollisionResponse::Block),
      std::make_pair(ColliderType::Enemy, CollisionResponse::Block),
      std::make_pair(ColliderType::PlayerProjectile, CollisionResponse::Block));

    _collisionProfileManager->SetProfileResponse(
      "PlayerProjectile", std::make_pair(ColliderType::Enemy, CollisionResponse::Block));

    _collisionProfileManager->SetProfileResponse(
      "EnemyProjectile", std::make_pair(ColliderType::Player, CollisionResponse::Block));

    _collisionProfileManager->SetProfileResponse(
      "Item", std::make_pair(ColliderType::Player, CollisionResponse::Block));
#endif // _HAS_COLLISION_MODULE

    // TODO save & load

    _currentLevel = CreateLevel<SnowbrosLevel>(This<World>(), name);
}

void World::Destroy()
{
    for (auto& [_, level] : _levels)
        DESTROY(level)

    DESTROY(_currentLevel)
}

void World::Tick(float deltaTime)
{
    if (_currentLevel)
        _currentLevel->Tick(deltaTime);
}

void World::Collision(float deltaTime)
{
    if (_currentLevel)
        _currentLevel->Collision(deltaTime);
}

void World::RenderUI(float deltaTime)
{
    // if (_currentLevel)
    //     _currentLevel->RenderUI(deltaTime);
}
#ifdef _HAS_COLLISION_MODULE
Ptr<CollisionProfileManager> World::GetCollisionProfileManager() const
{
    return _collisionProfileManager;
}
#endif // _HAS_COLLISION_MODULE
Ptr<Level> World::GetCurrentLevel() const
{
    return _currentLevel;
}

Ptr<Actor> World::GetPlayer() const
{
    return Lock<Actor>(_player);
}

void World::SetMainPlayer(Ptr<Actor> player) {}
