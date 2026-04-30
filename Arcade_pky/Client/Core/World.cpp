#include "pch.h"

#include "World.h"

#include "CollisionProfileManager.h"

#include "Level.h"
#include "Test/TestLevel.h"

void World::Init(const std::string& name)
{
    _collisionProfileManager = New<CollisionProfileManager>();
    _collisionProfileManager->Init();

    // TODO Create Collision Profiles

    // TODO save & load
    // TODO _currentLevel = CreateLevel<Level>(name);
    _currentLevel = CreateLevel<TestLevel>(This<World>(), name);
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

Ptr<class CollisionProfileManager> World::GetCollisionProfileManager() const
{
    return _collisionProfileManager;
}

Ptr<class Level> World::GetCurLevel() const
{
    return _currentLevel;
}

Ptr<class Actor> World::GetPlayer() const
{
    return Lock<Actor>(_player);
}

void World::SetMainPlayer(Ptr<class Actor> player) {}
