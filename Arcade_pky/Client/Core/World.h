#pragma once
#include "Object.h"

class World : public Object
{
public:
    World()           = default;
    ~World() override = default;
    DELETE_SPECIAL_FUNC(World)

public:
    Ptr<class CollisionProfileManager>                _collisionProfileManager;
    Ptr<class Level>                                  _currentLevel;
    std::unordered_map<std::string, Ptr<class Level>> _levels;
    Weak<class Actor>                                 _player;

public:
    virtual void Init(const std::string& name);
    void         Destroy() override;

    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void RenderUI(float deltaTime);

    Ptr<class CollisionProfileManager> GetCollisionProfileManager() const;
    Ptr<class Level>                   GetCurLevel() const;
    Ptr<class Actor>                   GetPlayer() const;

    void SetMainPlayer(Ptr<class Actor> player);

public:
    template <typename T>
    Ptr<T> CreateLevel(Ptr<class World> world, const std::string& path)
    {
        Ptr<T> level = New<T>();
        if (false == level->Init(world, path))
        {
            DESTROY(level)
            return nullptr;
        }

        return level;
    }
};
