#include "pch.h"

#include "SnowbrosEnemyState.h"

#include "Core/TimeManager.h"

SnowbrosEnemyState::SnowbrosEnemyState(EnemyStateType stateType) : _stateType(stateType) {}

SnowbrosEnemyState::EnemyStateType SnowbrosEnemyState::GetStateType() const
{
    return _stateType;
}
