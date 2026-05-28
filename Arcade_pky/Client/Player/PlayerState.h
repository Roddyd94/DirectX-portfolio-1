#pragma once
#include "PlayerComponent.h"
#include "Core/Collision/CollisionSystem.h"
#include "Core/Input/InputAction.h"
#include "Core/Input/Types.h"
#include "Core/Object.h"

class PlayerState : public Object
{
public:
    PlayerState()           = default;
    ~PlayerState() override = default;

protected:
    uint8 _stateType = -1;

public:
    void Destroy() override;

    virtual Ptr<PlayerState> HandleInput(
      Ptr<class PlayerComponent> player, Ptr<InputAction> action, ButtonEventType::Type buttonEvent)
      = 0;

    virtual void Enter(Ptr<class PlayerComponent> player) {}
    virtual void Exit(Ptr<class PlayerComponent> player) {}
    virtual void Tick(Ptr<class PlayerComponent> player, float deltaTime) {}

    virtual void CollideWith(Ptr<class PlayerComponent> playerComponent,
      CollisionState::Type                              collisionType,
      Weak<class CollisionComponent>                    collider)
    {
    }

    uint8 GetType() const;
};
