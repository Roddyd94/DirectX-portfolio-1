#pragma once
#include "PlayerState.h"

namespace snowbros
{
    class PlayerStateGround : public PlayerState
    {
    public:
        PlayerStateGround()           = default;
        ~PlayerStateGround() override = default;

    public:
        Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> player,
          Ptr<class InputAction>                   action,
          ButtonEventType::Type                    buttonEvent) override;

        void Enter(Ptr<class PlayerComponent> player) override;
        void Exit(Ptr<class PlayerComponent> player) override;
        void Tick(Ptr<class PlayerComponent> player, float deltaTime) override;
    };
} // namespace snowbros
