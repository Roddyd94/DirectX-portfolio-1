#include "pch.h"

#include "PlayerState.h"

#include "PlayerStateDead.h"
#include "PlayerStateGround.h"
#include "PlayerStateMidair.h"
#include "PlayerStateSnowball.h"

Ptr<PlayerStateGround>   PlayerState::ground   = New<PlayerStateGround>();
Ptr<PlayerStateSnowball> PlayerState::snowball = New<PlayerStateSnowball>();

void            PlayerState::Destroy() {}
PlayerStateType PlayerState::GetType() const
{
    return _stateType;
}
