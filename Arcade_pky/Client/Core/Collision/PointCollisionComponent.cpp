#include "pch.h"

#include "PointCollisionComponent.h"

#include "Core/ResourceManager.h"

#include "AABBCollisionComponent.h"
#include "CollisionSystem.h"
#include "OBBCollisionComponent.h"
#include "SphereCollisionComponent.h"
#include "Core/Level.h"

#ifdef _DEBUG
#include "Core/InstanceRenderer.h"
#include "Core/InstanceRendererComponent.h"
#endif // _DEBUG

PointCollisionComponent::PointCollisionComponent()
{
    _shape = CollisionShape::Point;
}

PointCollisionComponent::~PointCollisionComponent() {}

bool PointCollisionComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    CollisionComponent::Init(componentID, name, owner);

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return false;

#if _DEBUG
    auto renderer = level->FindInstanceRenderer("SphereCollider");
    if (renderer)
        _renderer = renderer->GetRendererComponent();
#endif // _DEBUG

    return true;
}

void PointCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}

void PointCollisionComponent::Tick(float deltaTime)
{
    CollisionComponent::Tick(deltaTime);

    SetWorldScale({0.1f, 0.1f, 1.f});
}

void PointCollisionComponent::Collision(float deltaTime)
{
    CollisionComponent::Collision(deltaTime);
}

void PointCollisionComponent::Render(float deltaTime)
{
    CollisionComponent::Render(deltaTime);
}

bool PointCollisionComponent::Collision(Weak<CollisionComponent> other)
{
    Ptr<CollisionComponent> otherCollider = Lock<CollisionComponent>(other);
    if (nullptr == otherCollider)
        return false;

    switch (otherCollider->GetShape())
    {
    case CollisionShape::AABB:
    {
        return CollisionSystem::AABBToPoint(
          Cast<CollisionComponent, AABBCollisionComponent>(otherCollider),
          This<PointCollisionComponent>());
    }
    break;
    case CollisionShape::OBB:
    {
        return CollisionSystem::OBBToPoint(
          Cast<CollisionComponent, OBBCollisionComponent>(otherCollider),
          This<PointCollisionComponent>());
    }
    break;
    case CollisionShape::Sphere:
    {
        return CollisionSystem::SphereToPoint(
          Cast<CollisionComponent, SphereCollisionComponent>(otherCollider),
          This<PointCollisionComponent>());
    }
    break;
    case CollisionShape::Point:
    {
        return CollisionSystem::PointToPoint(This<PointCollisionComponent>(),
          Cast<CollisionComponent, PointCollisionComponent>(otherCollider));
    }
    break;
    default:
        break;
    }

    return false;
}
