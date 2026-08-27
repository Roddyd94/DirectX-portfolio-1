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
    struct MoveProfile
    {
        int32 tryMoveCallCount   = 0;
        int32 findSnowballsCount = 0;
        int32 snowballStayCount  = 0;
        int32 maxDepth           = 0;
    };

private:
    inline static MoveProfile _moveProfile;

public:
    bool TryMoveX(float deltaX, int32 depth = 0);
    void Throw(int32 playerNumber, float direction);

    static void        ResetMoveProfile();
    static MoveProfile GetMoveProfile();

public:
    bool Init(Ptr<class AIComponent> owner) override;
    void Destroy() override;

    virtual void ChangeAnimationClip(SnowbrosEnemyAnimationType type, bool play = true) = 0;
    virtual void AddNotifyToAnimationClipEnd(
      SnowbrosEnemyAnimationType type, std::function<void()>&& func) = 0;

protected:
    bool CheckPatrolPoint();

public:
    static void FindSnowballs(Ptr<class CollisionManager> collisionManager,
      std::vector<Weak<class AABBCollisionComponent>>&    snowballs);
};
