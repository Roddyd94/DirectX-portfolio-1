#pragma once
#include "AI/AIBlackboard.h"

class MonkeyBlackboard : public AIBlackboard
{
    // AIBlackboard을(를) 통해 상속됨
    void SetDirection(float direction) override;
};
