#pragma once
#include "SnowballMorphableEnemyStateMachine.h"

class SpawnStateMachine : public SnowballMorphableEnemyStateMachine
{
public:
    SpawnStateMachine()           = default;
    ~SpawnStateMachine() override = default;

public:
    bool Init(Ptr<class AIComponent> owner) override;
    void Destroy() override;

    void ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play = true) override;
    void AddNotifyToAnimationClipEnd(
      SnowbrosEnemyAnimationType type, std::function<void()>&& func) override;
};
