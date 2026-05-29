#include "pch.h"

#include "SnowbrosEnemyState.h"

SnowbrosEnemyState::SnowbrosEnemyState(EnemyStateType stateType) : _stateType(stateType) {}

SnowbrosEnemyState::EnemyStateType SnowbrosEnemyState::GetStateType() const
{
    return _stateType;
}
