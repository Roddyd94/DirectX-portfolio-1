#include "pch.h"

#include "PlayerStateGround.h"

namespace snowbros
{
    Ptr<PlayerState> snowbros::PlayerStateGround::HandleInput(Ptr<class PlayerComponent> player,
      Ptr<class InputAction>                                                             action,
      ButtonEventType::Type buttonEvent)
    {
        return nullptr;
    }

    void snowbros::PlayerStateGround::Enter(Ptr<class PlayerComponent> player) {}

    void snowbros::PlayerStateGround::Exit(Ptr<class PlayerComponent> player) {}

    void snowbros::PlayerStateGround::Tick(Ptr<class PlayerComponent> player, float deltaTime) {}

} // namespace snowbros
