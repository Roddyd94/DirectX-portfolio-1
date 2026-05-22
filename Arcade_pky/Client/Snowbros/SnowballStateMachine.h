#pragma once
#include "SnowballState.h"
#include "Core/Collision/CollisionSystem.h"
#include "Core/Input/Types.h"
#include "Core/Object.h"

class SnowballStateMachine : public Object
{
public:
    SnowballStateMachine()           = default;
    ~SnowballStateMachine() override = default;

private:
    Weak<class SnowballComponent> _owner;
    Ptr<class SnowballState>      _currentState = nullptr;

public:
    void Init(Ptr<class SnowballComponent> snowball, Ptr<class SnowballState> state);
    void Destroy() override;
    void Tick(float deltaTime);

    void Transition(Ptr<class SnowballState> state);

    bool TryKick(float direction);
    bool TryPush(float direction);
    void CollideWith(CollisionState::Type collisionState, Weak<class CollisionComponent> collider);

    Ptr<class SnowballComponent>  GetOwner() const;
    SnowballStateType             GetCurrentStateType() const;
};
