#include "pch.h"

#include "CollisionComponent.h"

#include "CollisionProfileManager.h"

#include "Core/Actor.h"
#include "Core/Level.h"

#ifdef _DEBUG
#include "Core/InstanceRenderer.h"
#include "Core/InstanceRendererComponent.h"
#include "Tilemap/TileOutlineStructureBuffer.h"
#endif // _DEBUG

CollisionComponent::CollisionComponent()
{
#ifdef _DEBUG
    _shouldRender    = true;
    _renderLayerName = "Collider";
#endif // _DEBUG
}

CollisionComponent::~CollisionComponent() {}

bool CollisionComponent::Init(int32 componentID, const std::string& name, Ptr<Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    _colliderID = {owner->GetActorID(), GetComponentID()};

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return false;

    level->AddCollision(This<CollisionComponent>());

    return true;
}

void CollisionComponent::Destroy()
{
    SceneComponent::Destroy();

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    level->RemoveCollision(This<CollisionComponent>());

    for (auto& [colliderInfo, collisionStatesOfType] : _collisionStates)
    {
        auto& [colliderType, colliderID] = colliderInfo;

        Ptr<CollisionComponent> foundCollider = level->FindCollider(colliderType, colliderID);
        if (nullptr == foundCollider)
            continue;

        foundCollider->RemoveCollisionState(GetColliderType(), _colliderID);
    }
}

void CollisionComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void CollisionComponent::Collision(float deltaTime)
{
    SceneComponent::Collision(deltaTime);
}

void CollisionComponent::PreRender(float deltaTime)
{
    SceneComponent::PreRender(deltaTime);

#ifdef _DEBUG
    Vector4 color;
    if (_collisionStates.size() > 0)
        color = {1.f, 0.f, 0.f, 1.f};
    else
        color = {0.f, 1.f, 0.f, 1.f};

    Vector2 worldPos = GetWorldPosition().ToVector2();
    Vector2 size     = GetWorldScale().ToVector2();

    auto renderer = Lock(_renderer);
    renderer->AddData<TileOutlineStructureBuffer>(color, worldPos, size);
#endif // _DEBUG
}

// void CollisionComponent::Render(float deltaTime)
//{
//     SceneComponent::Render(deltaTime);
//
// #if _DEBUG
//     if (!_mesh || !_shader)
//         return;
//
//     _transformConstantBuffer->SetWorldMatrix(_matrix.world);
//
//     Ptr<Level> level = Lock<Level>(_level);
//     if (nullptr == level)
//         return;
//
//     _transformConstantBuffer->SetViewMatrix(level->GetViewMatrix());
//     _transformConstantBuffer->SetProjMatrix(level->GetProjMatrix());
//
//     _transformConstantBuffer->Update();
//
//     if (_collisionStates.size() > 0)
//         _colorConstantBuffer->SetData(1.f, 0.f, 0.f, 1.f);
//     else
//         _colorConstantBuffer->SetData(0.f, 1.f, 0.f, 1.f);
//
//     _colorConstantBuffer->Update();
//
//     _shader->SetShader();
//     _mesh->Render();
// #endif // _DEBUG
// }

CollisionState::Type CollisionComponent::CheckCollisionState(
  Ptr<class CollisionComponent> otherCollider) const
{
    return CheckCollisionState(otherCollider->GetColliderType(), otherCollider->_colliderID);
}

CollisionState::Type CollisionComponent::CheckCollisionState(
  ColliderType::Type otherColliderType, ComponentIDPair otherColliderID) const
{
    auto it = _collisionStates.find({otherColliderType, otherColliderID});

    if (_collisionStates.end() == it)
        return CollisionState::Exit;

    return it->second;
}

Ptr<CollisionProfile> CollisionComponent::GetProfile() const
{
    return _profile;
}

ColliderType::Type CollisionComponent::GetColliderType() const
{
    if (nullptr == _profile)
        return ColliderType::End;

    return _profile->GetColliderType();
}

ComponentIDPair CollisionComponent::GetColliderID() const
{
    return _colliderID;
}

void CollisionComponent::SetCollisionProfile(const std::string& name)
{
    Ptr<Level> level = GetLevel();

    Ptr<CollisionProfile> profileFound = level->GetCollisionProfileManager()->FindProfile(name);
    if (nullptr == profileFound)
        return;

    level->RemoveCollision(This<CollisionComponent>());
    _profile = profileFound;
    level->AddCollision(This<CollisionComponent>());
}

void CollisionComponent::AddCollisionState(
  ColliderType::Type colliderType, ComponentIDPair colliderID, CollisionState::Type stateType)
{
    _collisionStates[{colliderType, colliderID}] = stateType;
}

void CollisionComponent::RemoveCollisionState(
  ColliderType::Type colliderType, ComponentIDPair colliderID)
{
    if (_collisionStates.empty())
        return;

    _collisionStates.erase({colliderType, colliderID});
}

bool CollisionComponent::Collision(Weak<CollisionComponent> other)
{
    return false;
}

void CollisionComponent::Invoke(CollisionState::Type state, Weak<CollisionComponent> other)
{
    Ptr<CollisionComponent> otherCollider = Lock<CollisionComponent>(other);
    if (nullptr == otherCollider)
        return;

    ColliderType::Type otherColliderType = otherCollider->GetColliderType();
    ComponentIDPair    otherColliderID   = otherCollider->GetColliderID();
    if (state == CollisionState::Enter || state == CollisionState::Stay)
        AddCollisionState(otherColliderType, otherColliderID, state);
    else
        RemoveCollisionState(otherColliderType, otherColliderID);

    if (_collisionCallback[state])
        _collisionCallback[state](otherCollider);
}
