#pragma once
#include "SnowballMorphableEnemyStateMachine.h"

class GoblinStateMachine : public SnowballMorphableEnemyStateMachine
{
public:
    GoblinStateMachine()           = default;
    ~GoblinStateMachine() override = default;

public:
    bool Init(Ptr<class AIComponent> owner) override;
    void Destroy() override;

    void ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play = true) override;
    void AddNotifyToAnimationClipEnd(
      SnowbrosEnemyAnimationType type, std::function<void()>&& func) override;
};
