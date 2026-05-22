#pragma once
#include "Core/Collision/CollisionSystem.h"
#include "Core/Input/InputAction.h"
#include "Core/Input/Types.h"
#include "Core/Object.h"

enum class SnowballStateType
{
    Forming,
    Formed,
    Rolling,
    End
};

class SnowballState : public Object
{
public:
    SnowballState()           = default;
    ~SnowballState() override = default;

protected:
    SnowballStateType _stateType = SnowballStateType::End;

public:
    void Destroy() override;

    virtual void Enter(Ptr<class SnowballComponent> snowball) {}
    virtual void Exit(Ptr<class SnowballComponent> snowball) {}
    virtual void Tick(Ptr<class SnowballComponent> snowball, float deltaTime) {}

    virtual bool TryKick(Ptr<class SnowballComponent> snowball, float direction) { return false; }
    virtual bool TryPush(Ptr<class SnowballComponent> snowball, float direction) { return false; }
    virtual void CollideWith(Ptr<class SnowballComponent> snowball,
      CollisionState::Type                                collisionState,
      Weak<class CollisionComponent>                      collider);

    void HandleGravity(Ptr<class SnowballComponent> snowball, float deltaTime);

    SnowballStateType GetType() const;
};
