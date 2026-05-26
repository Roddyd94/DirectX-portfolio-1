#pragma once
#include "PlayerBlackboard.h"
#include "PlayerState.h"
#include "Core/Collision/CollisionSystem.h"
#include "Core/Input/Types.h"
#include "Core/Object.h"

class PlayerStateMachine : public Object
{
public:
    PlayerStateMachine()           = default;
    ~PlayerStateMachine() override = default;

private:
    Weak<class PlayerComponent> _owner;
    Ptr<class PlayerBlackboard> _blackboard   = nullptr;
    Ptr<class PlayerState>      _currentState = nullptr;

public:
    virtual void Init(Ptr<class PlayerComponent> player, Ptr<class PlayerState> state);
    void         Destroy() override;
    void         Tick(float deltaTime);

    void Transition(Ptr<class PlayerState> state);
    void HandleInput(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);

    Ptr<class PlayerComponent> GetOwner() const;
    uint8                      GetCurrentStateType() const;

    void SetBlackboard(Ptr<class PlayerBlackboard> blackboard);

    void CollideWith(CollisionState::Type collisionType, Weak<class CollisionComponent> collider);

public:
    template <typename T>
    Ptr<T> GetBlackboard() const
    {
        return Cast<PlayerBlackboard, T>(_blackboard);
    }
};
