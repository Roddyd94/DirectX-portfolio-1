#include "pch.h"

#include "PlayerState.h"

void PlayerState::Destroy() {}

uint8 PlayerState::GetType() const
{
    return _stateType;
}
