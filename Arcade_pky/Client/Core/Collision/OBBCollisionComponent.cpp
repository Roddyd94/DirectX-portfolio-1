#include "pch.h"

#include "OBBCollisionComponent.h"

#include "Core/ResourceManager.h"

#include "AABBCollisionComponent.h"
#include "CollisionSystem.h"
#include "SphereCollisionComponent.h"

OBBCollisionComponent::OBBCollisionComponent()
{
    _shape = CollisionShape::OBB;
}

OBBCollisionComponent::~OBBCollisionComponent() {}

bool OBBCollisionComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    CollisionComponent::Init(componentID, name, owner);

#ifdef _DEBUG
    _mesh = MESH_LINE_RECT;
#endif //  _DEBUG

    return true;
}

void OBBCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}

void OBBCollisionComponent::Tick(float deltaTime)
{
    CollisionComponent::Tick(deltaTime);

    _box.center.x = _world.position.x;
    _box.center.y = _world.position.y;

    _box.axises[AxisType::X].x = _axis[AxisType::X].x;
    _box.axises[AxisType::X].y = _axis[AxisType::X].y;

    _box.axises[AxisType::Y].x = _axis[AxisType::Y].x;
    _box.axises[AxisType::Y].y = _axis[AxisType::Y].y;

    _box.halfSize = _boxSize * 0.5f;

    Vector2 pos[4];
    pos[0] = _box.center - _box.axises[AxisType::X] * _box.halfSize.x
           + _box.axises[AxisType::Y] * _box.halfSize.y;
    pos[1] = _box.center - _box.axises[AxisType::X] * _box.halfSize.x
           - _box.axises[AxisType::Y] * _box.halfSize.y;
    pos[2] = _box.center + _box.axises[AxisType::X] * _box.halfSize.x
           + _box.axises[AxisType::Y] * _box.halfSize.y;
    pos[3] = _box.center + _box.axises[AxisType::X] * _box.halfSize.x
           - _box.axises[AxisType::Y] * _box.halfSize.y;

    _min.x = pos[0].x;
    _min.y = pos[0].y;

    _max.x = pos[0].x;
    _max.y = pos[0].y;

    for (size_t i = 0; i < 4; i++)
    {
        _min.x = _min.x > pos[i].x ? pos[i].x : _min.x;
        _min.y = _min.y > pos[i].y ? pos[i].y : _min.y;
        _max.x = _max.x < pos[i].x ? pos[i].x : _max.x;
        _max.y = _max.y < pos[i].y ? pos[i].y : _max.y;

        SetWorldScale({_boxSize.x, _boxSize.y, 1.f});
    }
}

void OBBCollisionComponent::Collision(float deltaTime)
{
    CollisionComponent::Collision(deltaTime);
}

void OBBCollisionComponent::Render(float deltaTime)
{
    CollisionComponent::Render(deltaTime);
}

bool OBBCollisionComponent::Collision(Weak<CollisionComponent> other)
{
    Ptr<CollisionComponent> otherCollider = Lock(other);
    if (nullptr == otherCollider)
        return false;

    switch (otherCollider->GetShape())
    {
    case CollisionShape::AABB:
    {
        return CollisionSystem::AABBToOBB(
          Cast<CollisionComponent, AABBCollisionComponent>(otherCollider),
          This<OBBCollisionComponent>());
    }
    break;
    case CollisionShape::OBB:
    {
        return CollisionSystem::OBBToOBB(This<OBBCollisionComponent>(),
          Cast<CollisionComponent, OBBCollisionComponent>(otherCollider));
    }
    break;
    case CollisionShape::Sphere:
    {
        return CollisionSystem::OBBToSphere(This<OBBCollisionComponent>(),
          Cast<CollisionComponent, SphereCollisionComponent>(otherCollider));
    }
    break;
    }

    return false;
}

void OBBCollisionComponent::AttachToComponent(Ptr<SceneComponent> comp)
{
    Vector3 scale = comp->GetWorldScale();
    SetBoxSize(scale.x, scale.y);
    SceneComponent::AttachToComponent(comp);
}
