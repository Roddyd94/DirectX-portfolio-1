#pragma once
#include "AI/AIStateMachine.h"

enum class SnowbrosEnemyAnimationType
{
    Walk,
    Turn,
    Jump,
    Midair,
    Crouch,
    Awake,
    Struggle,
    Dizzy,
    Launched,
    Airborne,
    Dead,
    Standing,
    Shoot,
    Fly,
    None
};

class SnowballMorphableEnemyStateMachine : public AIStateMachine
{
public:
    bool TryMoveX(float deltaX);
    void Throw(float direction);

public:
    bool Init(Ptr<class AIComponent> owner) override;

    virtual void ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play = true) = 0;
    virtual void AddNotifyToAnimationClipEnd(
      SnowbrosEnemyAnimationType type, std::function<void()>&& func)
      = 0;

public:
    static void FindSnowballs(Ptr<class CollisionManager> collisionManager,
      std::vector<Weak<class AABBCollisionComponent>>&    snowballs);
};
