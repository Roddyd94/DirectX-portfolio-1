#include "pch.h"

#include "BossState.h"

BossState::BossState(EnemyStateType stateType) : SnowbrosEnemyState(stateType) {}

void BossState::OnTick(float deltaTime)
{
    SnowbrosEnemyState::OnTick(deltaTime);

    if (_callbackSpriteEffect)
    {
        int32 unitFrameCounts = static_cast<int32>(TimeManager::targetFPS * 0.2f);
        int32 frameNumber     = TimeManager::Instance().GetFrameCount() % unitFrameCounts;
        if (frameNumber > unitFrameCounts / 2)
            _callbackSpriteEffect(false);
        else
            _callbackSpriteEffect(true);
    }
}
