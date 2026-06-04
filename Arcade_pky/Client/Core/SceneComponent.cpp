#include "pch.h"

#include "SceneComponent.h"

#include "RenderManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"

#include "Actor.h"
#include "TransformConstantBuffer.h"

SceneComponent::SceneComponent() : _renderLayerName("Default"), _shouldRender(false) {}

SceneComponent::~SceneComponent() {}

bool SceneComponent::Init(int32 componentID, const std::string& name, Ptr<Actor> owner)
{
    if (!Component::Init(componentID, name, owner))
        return false;

    _transformConstantBuffer = CONSTANT_BUFFER_TRANSFORM;

    if (_shouldRender)
        RenderManager::Instance().AddRenderComponent(owner->GetActorID(), This<SceneComponent>());

    return true;
}

void SceneComponent::Destroy()
{
    for (auto& [_, child] : _children)
        DESTROY(child)

    if (_shouldRender)
    {
        Ptr<Actor> owner = Lock<Actor>(_owner);
        if (!owner)
            return;

        RenderManager::Instance().RemoveRenderComponent(
          _renderLayerName, owner->GetActorID(), GetComponentID());
    }

    _children.clear();
}

void SceneComponent::Tick(float deltaTime)
{
    for (auto& [_, child] : _children)
    {
        if (nullptr == child)
            continue;

        if (!child->IsActive() || !child->IsEnable())
            continue;

        child->Tick(deltaTime);
    }
}

void SceneComponent::Collision(float deltaTime) {}

void SceneComponent::PreRender(float deltaTime) {}

void SceneComponent::Render(float deltaTime) {}

void SceneComponent::SetEnable(bool enable)
{
    Object::SetEnable(enable);

    for (auto& [_, child] : _children)
        child->SetEnable(enable);
}

Ptr<SceneComponent> SceneComponent::GetParent() const
{
    return Lock<SceneComponent>(_parent);
}

Ptr<SceneComponent> SceneComponent::FindComponent(int32 componentID) const
{
    if (auto it = _children.find(componentID); _children.end() != it)
        return it->second;

    for (auto& [_, child] : _children)
    {
        Ptr<SceneComponent> comp = child->FindComponent(componentID);
        if (nullptr != comp)
            return comp;
    }

    return nullptr;
}

const std::string& SceneComponent::GetRenderLayerName() const
{
    return _renderLayerName;
}

const Transform& SceneComponent::GetWorldTransform() const
{
    return _world;
}

Vector3 SceneComponent::GetWorldScale() const
{
    return _world.scale;
}

Vector3 SceneComponent::GetWorldPosition() const
{
    return _world.position;
}

Vector3 SceneComponent::GetWorldRotation() const
{
    return _world.rotation;
}

const Transform& SceneComponent::GetRelativeTransform() const
{
    return _relative;
}

Vector3 SceneComponent::GetRelativeScale() const
{
    return _relative.scale;
}

Vector3 SceneComponent::GetRelativePosition() const
{
    return _relative.position;
}

Vector3 SceneComponent::GetRelativeRotation() const
{
    return _relative.rotation;
}

void SceneComponent::AddChild(Ptr<SceneComponent> comp)
{
    if (nullptr == comp)
        return;

    if (comp->GetComponentID() == GetComponentID())
        return;

    comp->_parent        = This<SceneComponent>();
    _children[comp->_id] = comp;

    UpdateTransform();
}

void SceneComponent::AttachToComponent(Ptr<SceneComponent> comp)
{
    comp->AddChild(This<SceneComponent>());
}

void SceneComponent::SetRenderLayer(const std::string& name)
{
    _renderLayerName = name;

    int32 actorID = GetOwner()->GetActorID();
    RenderManager::Instance().AddRenderComponent(actorID, This<SceneComponent>());
    RenderManager::Instance().RefreshLayer();
}

void SceneComponent::SetWorldTransform(const Transform& transform)
{
    SetWorldTransform(transform.position, transform.scale, transform.rotation);
}

void SceneComponent::SetWorldTransform(Vector3 position, Vector3 scale, Vector3 rotation)
{
    SetWorldScale(scale);
    SetWorldPosition(position);
    SetWorldRotation(rotation);
}

void SceneComponent::SetWorldScale(Vector3 scale)
{
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
    {
        _relative.scale.x = scale.x / parentComp->_world.scale.x;
        _relative.scale.y = scale.y / parentComp->_world.scale.y;
        _relative.scale.z = scale.z / parentComp->_world.scale.z;
    }
    else
    {
        _relative.scale.x = scale.x;
        _relative.scale.y = scale.y;
        _relative.scale.z = scale.z;
    }

    UpdateTransform();
}

void SceneComponent::SetWorldScale(Vector2 scale)
{
    SetWorldScale({scale.x, scale.y, _world.scale.z});
}

void SceneComponent::SetWorldPosition(Vector3 position)
{
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
    {
        Matrix matParentInv = parentComp->_matrix.world;
        matParentInv.Inverse();
        _relative.position
          = Vector3(position.x, position.y, position.z).TransformCoord(matParentInv);
    }
    else
    {
        _relative.position.x = position.x;
        _relative.position.y = position.y;
        _relative.position.z = position.z;
    }

    UpdateTransform();
}

void SceneComponent::SetWorldPosition(Vector2 position)
{
    SetWorldPosition({position.x, position.y, _world.position.z});
}

void SceneComponent::SetWorldRotation(Vector3 rotation)
{
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
    {
        _relative.rotation.x = rotation.x - parentComp->_world.rotation.x;
        _relative.rotation.y = rotation.y - parentComp->_world.rotation.y;
        _relative.rotation.z = rotation.z - parentComp->_world.rotation.z;
    }
    else
    {
        _relative.rotation.x = rotation.x;
        _relative.rotation.y = rotation.y;
        _relative.rotation.z = rotation.z;
    }

    UpdateTransform();
}

void SceneComponent::SetWorldRotation(Vector2 rotation)
{
    SetWorldRotation({rotation.x, rotation.y, _world.rotation.z});
}

void SceneComponent::SetRelativeTransform(const Transform& transform)
{
    SetRelativeTransform(transform.position, transform.scale, transform.rotation);
}

void SceneComponent::SetRelativeTransform(Vector3 position, Vector3 scale, Vector3 rotation)
{
    SetRelativePosition(position);
    SetRelativeScale(scale);
    SetRelativeRotation(rotation);
}

void SceneComponent::SetRelativeScale(Vector3 scale)
{
    _relative.scale = scale;
    UpdateTransform();
}

void SceneComponent::SetRelativeScale(Vector2 scale)
{
    SetRelativeScale({scale.x, scale.y, _relative.scale.z});
}

void SceneComponent::SetRelativePosition(Vector3 position)
{
    _relative.position = position;
    UpdateTransform();
}

void SceneComponent::SetRelativePosition(Vector2 position)
{
    SetRelativePosition({position.x, position.y, _relative.position.z});
}

void SceneComponent::SetRelativeRotation(Vector3 rotation)
{
    _relative.rotation = rotation;
    UpdateTransform();
}

void SceneComponent::SetRelativeRotation(Vector2 rotation)
{
    SetRelativeRotation({rotation.x, rotation.y, _relative.rotation.z});
}

void SceneComponent::UpdateTransform()
{
    _matrix.SetSRT(_relative);

    Matrix matLocal = _matrix.scale * _matrix.rotation * _matrix.translation;
    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
        _matrix.world = matLocal * parentComp->_matrix.world;
    else
        _matrix.world = matLocal;

    if (Ptr<SceneComponent> parentComp = Lock<SceneComponent>(_parent))
        _world = _matrix.CalculateWorld(parentComp->_matrix.world);
    else
        _world = _matrix.CalculateWorld();

    for (auto& [_, child] : _children)
    {
        if (child)
            child->UpdateTransform();
    }

    Vector3 axis[AxisType::End] = {
      Vector3(Vector3::axisX),
      Vector3(Vector3::axisY),
      Vector3(Vector3::axisZ),
    };

    _axis[AxisType::X] = axis[AxisType::X].TransformNormal(_matrix.rotation);
    _axis[AxisType::Y] = axis[AxisType::Y].TransformNormal(_matrix.rotation);
    _axis[AxisType::Z] = axis[AxisType::Z].TransformNormal(_matrix.rotation);
}
