#include "pch.h"

#include "SnowbrosPlayerBlackboard.h"

#include "Core/TimeManager.h"

void SnowbrosPlayerBlackboard::Destroy()
{
    TimeManager::Instance().RemoveTimer(invincibleTimer);
    PlayerBlackboard::Destroy();
}
