#include "pch.h"

#include "PlayerStateSnowball.h"

namespace snowbros
{
    Ptr<PlayerState> snowbros::PlayerStateSnowball::HandleInput(Ptr<class PlayerComponent> player, Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
    {
        // if (action)
        
        return nullptr;
    }

    void snowbros::PlayerStateSnowball::Enter(Ptr<class PlayerComponent> player) {}

    void snowbros::PlayerStateSnowball::Exit(Ptr<class PlayerComponent> player) {}

    void snowbros::PlayerStateSnowball::Tick(Ptr<class PlayerComponent> player, float deltaTime) {}
} // namespace snowbros
