#include "pch.h"

#include "AABBCollisionComponent.h"

#include "Core/ResourceManager.h"

#include "CollisionSystem.h"
#include "OBBCollisionComponent.h"
#include "PointCollisionComponent.h"
#include "SphereCollisionComponent.h"
#include "Core/Actor.h"
#include "Core/Level.h"

#ifdef _DEBUG
#include "Core/InstanceRenderer.h"
#include "Core/InstanceRendererComponent.h"
#endif // _DEBUG

AABBCollisionComponent::AABBCollisionComponent()
{
    _shape = CollisionShape::AABB;
}

AABBCollisionComponent::~AABBCollisionComponent() {}

bool AABBCollisionComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    CollisionComponent::Init(componentID, name, owner);

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return false;

#if _DEBUG
    auto renderer = level->FindInstanceRenderer("SquareCollider");
    if (renderer)
        _renderer = renderer->GetRendererComponent();
#endif // _DEBUG

    return true;
}

void AABBCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}

void AABBCollisionComponent::Tick(float deltaTime)
{
    CollisionComponent::Tick(deltaTime);

    _box.left  = _world.position.x - _boxSize.x * 0.5f;
    _box.right = _world.position.x + _boxSize.x * 0.5f;

    _box.bottom = _world.position.y - _boxSize.y * 0.5f;
    _box.top    = _world.position.y + _boxSize.y * 0.5f;

    _min.x = _box.left;
    _max.x = _box.right;

    _min.y = _box.bottom;
    _max.y = _box.top;

    SetWorldScale({_boxSize.x, _boxSize.y, 1.f});
}

void AABBCollisionComponent::Collision(float deltaTime)
{
    CollisionComponent::Collision(deltaTime);
}

void AABBCollisionComponent::Render(float deltaTime)
{
    CollisionComponent::Render(deltaTime);
}

bool AABBCollisionComponent::Collision(Weak<CollisionComponent> other)
{
    Ptr<CollisionComponent> otherCollider = Lock<CollisionComponent>(other);
    if (nullptr == otherCollider)
        return false;

    switch (otherCollider->GetShape())
    {
    case CollisionShape::AABB:
    {
        return CollisionSystem::AABBToAABB(This<AABBCollisionComponent>(),
          Cast<CollisionComponent, AABBCollisionComponent>(otherCollider));
    }
    break;
    case CollisionShape::OBB:
    {
        return CollisionSystem::AABBToOBB(This<AABBCollisionComponent>(),
          Cast<CollisionComponent, OBBCollisionComponent>(otherCollider));
    }
    break;
    case CollisionShape::Sphere:
    {
        return CollisionSystem::AABBToSphere(This<AABBCollisionComponent>(),
          Cast<CollisionComponent, SphereCollisionComponent>(otherCollider));
    }
    break;
    case CollisionShape::Point:
    {
        return CollisionSystem::AABBToPoint(This<AABBCollisionComponent>(),
          Cast<CollisionComponent, PointCollisionComponent>(otherCollider));
    }
    break;
    default:
        break;
    }

    return false;
}

void AABBCollisionComponent::AttachToComponent(Ptr<SceneComponent> comp)
{
    Vector3 scale = comp->GetWorldScale();
    SetBoxSize(scale.x, scale.y);
    SceneComponent::AttachToComponent(comp);
}
