#pragma once
#include "Core/Input/InputAction.h"
#include "Core/Input/Types.h"
#include "Core/Object.h"

namespace snowbros
{
    class PlayerState : public Object
    {
    public:
        PlayerState()           = default;
        ~PlayerState() override = default;

    public:
        static Ptr<class PlayerStateGround>   ground;
        static Ptr<class PlayerStateSnowball> snowball;

    public:
        void Destroy() override;

        virtual Ptr<PlayerState> HandleInput(Ptr<class PlayerComponent> player,
          Ptr<InputAction>                                              action,
          ButtonEventType::Type                                         buttonEvent)
          = 0;

        virtual void Enter(Ptr<class PlayerComponent> player) {}
        virtual void Exit(Ptr<class PlayerComponent> player) {}
        virtual void Tick(Ptr<class PlayerComponent> player, float deltaTime) {}
    };
} // namespace snowbros
