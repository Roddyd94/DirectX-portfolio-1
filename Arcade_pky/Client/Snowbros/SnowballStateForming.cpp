#include "pch.h"

#include "SnowballStateForming.h"

#include "SnowballComponent.h"
#include "SnowballStateFormed.h"
#include "Core/Actor.h"
#include "Core/Animation/SpriteComponent.h"
#include "Core/Collision/CollisionComponent.h"
#include "Core/Collision/CollisionProfile.h"

SnowballStateForming::SnowballStateForming(float initialValue)
{
    _accValue = initialValue;
}

void SnowballStateForming::Enter(Ptr<class SnowballComponent> snowball) {}

void SnowballStateForming::Exit(Ptr<class SnowballComponent> snowball) {}

void SnowballStateForming::Tick(Ptr<class SnowballComponent> snowball, float deltaTime)
{
    Ptr<Actor> actor = snowball->GetOwner();

    _accValue -= decPerSecond * deltaTime;

    if (_accValue <= phaseThreshold[0])
    {
        snowball->OnDestroy();
        actor->SetActive(false);
        return;
    }

    for (int32 i = phaseCount - 1; i >= 0; i--)
    {
        if (_accValue > phaseThreshold[i])
        {
            auto sprite = actor->FindSceneComponent<SpriteComponent>("Root");
            sprite->SetFrameIndex(i);

            if (i == phaseCount - 1)
                snowball->Transition(New<SnowballStateFormed>(_accValue - phaseThreshold[i]));

            return;
        }
    }
}

bool SnowballStateForming::CanCollideWith(
  Ptr<class SnowballComponent> snowball, Weak<class CollisionComponent> collider)
{
    Ptr<CollisionComponent> colliderLock = Lock(collider);
    ColliderType::Type      colliderType = colliderLock->GetProfile()->GetColliderType();

    switch (colliderType)
    {
    case ColliderType::PlayerProjectile:
    case ColliderType::EnemyProjectile:
    case ColliderType::Snowball: // Rolling인 경우 파괴
        return true;
    }

    return false;
}
