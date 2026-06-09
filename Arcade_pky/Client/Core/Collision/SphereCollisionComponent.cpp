#include "pch.h"

#include "SphereCollisionComponent.h"

#include "Core/ResourceManager.h"

#include "AABBCollisionComponent.h"
#include "CollisionSystem.h"
#include "OBBCollisionComponent.h"
#include "PointCollisionComponent.h"
#include "Core/Level.h"

#ifdef _DEBUG
#include "Core/InstanceRenderer.h"
#include "Core/InstanceRendererComponent.h"
#endif // _DEBUG

SphereCollisionComponent::SphereCollisionComponent()
{
    _shape = CollisionShape::Sphere;
}

SphereCollisionComponent::~SphereCollisionComponent() {}

bool SphereCollisionComponent::Init(
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

void SphereCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}

void SphereCollisionComponent::Tick(float deltaTime)
{
    CollisionComponent::Tick(deltaTime);

    _min.x = _world.position.x - _radius;
    _max.x = _world.position.x + _radius;

    _min.y = _world.position.y - _radius;
    _max.y = _world.position.y + _radius;

    SetWorldScale({_radius * 2.f, _radius * 2.f, 1.f});
}

void SphereCollisionComponent::Collision(float deltaTime)
{
    CollisionComponent::Collision(deltaTime);
}

void SphereCollisionComponent::Render(float deltaTime)
{
    CollisionComponent::Render(deltaTime);
}

bool SphereCollisionComponent::Collision(Weak<CollisionComponent> other)
{
    Ptr<CollisionComponent> otherCollider = Lock<CollisionComponent>(other);
    if (nullptr == otherCollider)
        return false;

    switch (otherCollider->GetShape())
    {
    case CollisionShape::AABB:
    {
        return CollisionSystem::AABBToSphere(
          Cast<CollisionComponent, AABBCollisionComponent>(otherCollider),
          This<SphereCollisionComponent>());
    }
    break;
    case CollisionShape::OBB:
    {
        return CollisionSystem::OBBToSphere(
          Cast<CollisionComponent, OBBCollisionComponent>(otherCollider),
          This<SphereCollisionComponent>());
    }
    break;
    case CollisionShape::Sphere:
    {
        return CollisionSystem::SphereToSphere(This<SphereCollisionComponent>(),
          Cast<CollisionComponent, SphereCollisionComponent>(otherCollider));
    }
    break;
    case CollisionShape::Point:
    {
        return CollisionSystem::SphereToPoint(This<SphereCollisionComponent>(),
          Cast<CollisionComponent, PointCollisionComponent>(otherCollider));
    }
    break;
    default:
        break;
    }

    return false;
}

void SphereCollisionComponent::AttachToComponent(Ptr<SceneComponent> comp)
{
    Vector3 scale = comp->GetWorldScale();
    SetRadius(std::min(scale.x, scale.y));
    SceneComponent::AttachToComponent(comp);
}
