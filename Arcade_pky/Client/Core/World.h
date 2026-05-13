#pragma once
#include "Object.h"

class World : public Object
{
public:
    World()           = default;
    ~World() override = default;
    DELETE_SPECIAL_FUNC(World)

protected:
#ifdef _HAS_COLLISION_MODULE
    Ptr<class CollisionProfileManager> _collisionProfileManager;
#endif // _HAS_COLLISION_MODULE
    Ptr<class Level>                                  _currentLevel;
    std::unordered_map<std::string, Ptr<class Level>> _levels;
    Weak<class Actor>                                 _player;

public:
    virtual void Init(const std::string& name);
    void         Destroy() override;

    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void RenderUI(float deltaTime);

#ifdef _HAS_COLLISION_MODULE
    Ptr<class CollisionProfileManager> GetCollisionProfileManager() const;
#endif // _HAS_COLLISION_MODULE
    Ptr<class Level> GetCurrentLevel() const;
    Ptr<class Actor> GetPlayer() const;

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
