#include "pch.h"

#include "PlayerStateDead.h"

namespace snowbros
{
    Ptr<PlayerState> snowbros::PlayerStateDead::HandleInput(Ptr<class PlayerComponent> player, Ptr<InputAction> action, ButtonEventType::Type buttonEvent)
    {
        return nullptr;
    }

    void snowbros::PlayerStateDead::Enter(Ptr<class PlayerComponent> player) {}

    void snowbros::PlayerStateDead::Exit(Ptr<class PlayerComponent> player) {}

    void snowbros::PlayerStateDead::Tick(Ptr<class PlayerComponent> player, float deltaTime) {}
} // namespace snowbros
