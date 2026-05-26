#pragma once
#include "PlayerState.h"
#include "PlayerStateMachine.h"
#include "Core/ActorComponent.h"
#include "Core/Collision/CollisionSystem.h"

class PlayerComponent : public ActorComponent
{
public:
    PlayerComponent()           = default;
    ~PlayerComponent() override = default;

private:
    Ptr<class PlayerStateMachine> _stateMachine = nullptr;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;
    void Tick(float deltaTime) override;

    void Transition(Ptr<class PlayerState> state);
    void HandleInput(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);

    Ptr<class Player> GetPlayer() const;
    uint8             GetStateType() const;

    void SetBlackboard(Ptr<class PlayerBlackboard> blackboard);
    void CollideWith(CollisionState::Type collisionType, Weak<class CollisionComponent> collider);

public:
    template <typename T>
    Ptr<T> GetBlackboard() const
    {
        return _stateMachine->GetBlackboard<T>();
    }
};
