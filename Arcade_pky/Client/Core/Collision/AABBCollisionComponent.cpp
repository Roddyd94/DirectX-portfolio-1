#include "pch.h"

#include "AABBCollisionComponent.h"

#include "Core/ResourceManager.h"

#include "CollisionSystem.h"
#include "OBBCollisionComponent.h"
#include "SphereCollisionComponent.h"

AABBCollisionComponent::AABBCollisionComponent()
{
    _shape = CollisionShape::AABB;
}

AABBCollisionComponent::~AABBCollisionComponent() {}

bool AABBCollisionComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    CollisionComponent::Init(componentID, name, owner);

#ifdef _DEBUG
    _mesh = MESH_LINE_RECT;
#endif //  _DEBUG

    return true;
}

void AABBCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}

void AABBCollisionComponent::Tick(float deltaTime)
{
    CollisionComponent::Tick(deltaTime);

    _box.min.x = _world.position.x - _boxSize.x * 0.5f;
    _box.max.x = _world.position.x + _boxSize.x * 0.5f;

    _box.min.y = _world.position.y - _boxSize.y * 0.5f;
    _box.max.y = _world.position.y + _boxSize.y * 0.5f;

    _min.x = _box.min.x;
    _max.x = _box.max.x;

    _min.y = _box.min.y;
    _max.y = _box.max.y;

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
