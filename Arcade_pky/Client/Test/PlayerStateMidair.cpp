#include "pch.h"

#include "PlayerStateMidair.h"

namespace snowbros
{
    Ptr<PlayerState> snowbros::PlayerStateMidair::HandleInput(Ptr<class PlayerComponent> player, Ptr<class InputAction> action, ButtonEventType::Type buttonEvent)
    {
        return nullptr;
    }

    void snowbros::PlayerStateMidair::Enter(Ptr<class PlayerComponent> player) {}

    void snowbros::PlayerStateMidair::Exit(Ptr<class PlayerComponent> player) {}

    void snowbros::PlayerStateMidair::Tick(Ptr<class PlayerComponent> player, float deltaTime) {}
} // namespace snowbros
