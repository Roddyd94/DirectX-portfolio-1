#pragma once
#include "Core/ActorComponent.h"

class SnowballComponent : public ActorComponent
{
public:
    SnowballComponent()           = default;
    ~SnowballComponent() override = default;

private:
    std::function<void()>           _onDestroyCallback;
    Ptr<class SnowballStateMachine> _stateMachine = nullptr;
    Weak<class AIComponent>         _enemyComponent;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;
    void Tick(float deltaTime) override;

    void Transition(Ptr<class SnowballState> state);

    bool TryPush(float direction);
    bool TryKick(float direction);
    void CollideWith(Weak<class CollisionComponent> collider);

    void OnDestroy();

    Ptr<class AIComponent> GetEnemyComponent() const;

    void SetEnemyComponent(Ptr<class AIComponent> enemy);

public:
    template <typename T>
    void RegisterOnDestroyCallback(T* obj, void (T::*memFunc)())
    {
        _onDestroyCallback = std::bind(memFunc, obj);
    }

    template <typename T>
    void RegisterOnDestroyCallback(T&& func)
    {
        _onDestroyCallback = std::forward<T>(func);
    }
};
