#pragma once
#include "AI/AIStateMachine.h"

class SnowballMorphableEnemyStateMachine : public AIStateMachine
{
public:
    bool TryMoveX(float deltaX);

public:
    static void FindSnowballs(Ptr<class CollisionManager>     collisionManager,
      std::vector<Weak<class AABBCollisionComponent>>& snowballs);
};
