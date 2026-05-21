#include "pch.h"

#include "SnowballState.h"

void SnowballState::Destroy() {}

uint8 SnowballState::GetType() const
{
    return _stateType;
}
