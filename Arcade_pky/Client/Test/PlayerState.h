#pragma once
#include "Core/Input/InputAction.h"
#include "Core/Input/Types.h"
#include "Core/Object.h"

enum class PlayerStateType : uint8
{
    Ground,
    Midair,
    Snowball,
    Dead,
    End
};

class PlayerState : public Object
{
public:
    PlayerState()           = default;
    ~PlayerState() override = default;

protected:
    PlayerStateType _stateType = PlayerStateType::End;

public:
    static Ptr<class PlayerStateGround>   ground;
    static Ptr<class PlayerStateSnowball> snowball;

public:
    void Destroy() override;

    virtual Ptr<PlayerState> HandleInput(
      Ptr<class PlayerComponent> player, Ptr<InputAction> action, ButtonEventType::Type buttonEvent)
      = 0;

    virtual void Enter(Ptr<class PlayerComponent> player) {}
    virtual void Exit(Ptr<class PlayerComponent> player) {}
    virtual void Tick(Ptr<class PlayerComponent> player, float deltaTime) {}

    PlayerStateType GetType() const;
};
