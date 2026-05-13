#include "pch.h"

#include "PlayerState.h"

#include "PlayerStateDead.h"
#include "PlayerStateGround.h"
#include "PlayerStateMidair.h"
#include "PlayerStateSnowball.h"

namespace snowbros
{
    Ptr<PlayerStateGround>   PlayerState::ground   = New<PlayerStateGround>();
    Ptr<PlayerStateSnowball> PlayerState::snowball = New<PlayerStateSnowball>();

    void PlayerState::Destroy() {}
} // namespace snowbros
