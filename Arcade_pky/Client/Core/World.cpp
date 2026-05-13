#include "pch.h"

#include "World.h"

#include "Level.h"
#include "Test/TestLevel.h"

#ifdef _HAS_COLLISION_MODULE
#include "Core/Collision/CollisionProfileManager.h"
#endif // _HAS_COLLISION_MODULE

void World::Init(const std::string& name)
{
#ifdef _HAS_COLLISION_MODULE
    _collisionProfileManager = New<CollisionProfileManager>();
    _collisionProfileManager->Init();

    // TODO Create Collision Profiles
#endif // _HAS_COLLISION_MODULE

    // TODO save & load
    // TODO _currentLevel = CreateLevel<Level>(name);
    _currentLevel = CreateLevel<snowbros::TestLevel>(This<World>(), name);
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
