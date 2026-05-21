#pragma once
#include "Core/Input/InputAction.h"
#include "Core/Input/Types.h"
#include "Core/Object.h"

class SnowballState : public Object
{
public:
    SnowballState()           = default;
    ~SnowballState() override = default;

protected:
    uint8 _stateType = -1;

public:
    void Destroy() override;

    virtual void Enter(Ptr<class SnowballComponent> snowball) {}
    virtual void Exit(Ptr<class SnowballComponent> snowball) {}
    virtual void Tick(Ptr<class SnowballComponent> snowball, float deltaTime) {}

    virtual bool TryKick(Ptr<class SnowballComponent> snowball, float direction) { return false; }
    virtual bool TryPush(Ptr<class SnowballComponent> snowball, float direction) { return false; }
    virtual void CollideWith(
      Ptr<class SnowballComponent> snowball, Weak<class CollisionComponent> collider)
    {
    }

    uint8 GetType() const;
};
