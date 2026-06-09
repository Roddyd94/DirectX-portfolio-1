#pragma once
#include "SnowballMorphableEnemyStateMachine.h"

class SpitterStateMachine : public SnowballMorphableEnemyStateMachine
{
public:
    SpitterStateMachine()           = default;
    ~SpitterStateMachine() override = default;

public:
    bool Init(Ptr<class AIComponent> owner) override;
    void Destroy() override;

    void ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play = true) override;
    void AddNotifyToAnimationClipEnd(SnowbrosEnemyAnimationType type, std::function<void()>&& func);
};
