#include "pch.h"

#include "PlayerState.h"

#include "PlayerComponent.h"

void PlayerState::Destroy() {}

uint8 PlayerState::GetType() const
{
    return _stateType;
}
