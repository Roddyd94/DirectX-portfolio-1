#include "pch.h"

#include "CollisionComponent.h"

#include "CollisionProfileManager.h"

#include "Core/Actor.h"
#include "Core/Level.h"

#ifdef _DEBUG
#include "Core/ResourceManager.h"

#include "Core/ColorConstantBuffer.h"
#include "Core/Mesh.h"
#include "Core/Shader.h"
#include "Core/TransformConstantBuffer.h"
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

#if _DEBUG
    _shader                  = FIND_SHADER("FrameMeshShader", Shader);
    _transformConstantBuffer = CONSTANT_BUFFER_TRANSFORM;
    _colorConstantBuffer     = CONSTANT_BUFFER_COLOR;
#endif // _DEBUG

    return true;
}

void CollisionComponent::Destroy()
{
    SceneComponent::Destroy();

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    level->RemoveCollision(This<CollisionComponent>());

    for (auto& [colliderType, collisionStatesOfType] : _collisionStates)
    {
        for (auto& [colliderID, colliderState] : collisionStatesOfType)
        {
            Ptr<CollisionComponent> foundCollider = level->FindCollider(colliderType, colliderID);
            if (nullptr == foundCollider)
                continue;

            foundCollider->RemoveCollisionState(GetColliderType(), _colliderID);
        }
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

void CollisionComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

#if _DEBUG
    if (!_mesh || !_shader)
        return;

    _transformConstantBuffer->SetWorldMatrix(_matrix.world);

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    _transformConstantBuffer->SetViewMatrix(level->GetViewMatrix());
    _transformConstantBuffer->SetProjMatrix(level->GetProjMatrix());

    _transformConstantBuffer->Update();

    if (_collisionStates.size() > 0)
        _colorConstantBuffer->SetColor(1.f, 0.f, 0.f, 1.f);
    else
        _colorConstantBuffer->SetColor(0.f, 1.f, 0.f, 1.f);

    _colorConstantBuffer->Update();

    _shader->SetShader();
    _mesh->Render();
#endif // _DEBUG
}

CollisionState::Type CollisionComponent::CheckCollisionState(
  Ptr<class CollisionComponent> otherCollider) const
{
    return CheckCollisionState(otherCollider->GetColliderType(), otherCollider->_colliderID);
}

CollisionState::Type CollisionComponent::CheckCollisionState(
  ColliderType::Type otherColliderType, ComponentIDPair otherColliderID) const
{
    auto itType = _collisionStates.find(otherColliderType);

    if (_collisionStates.end() == itType)
        return CollisionState::Exit;

    auto& collisionStatesOfType = itType->second;

    auto itState = collisionStatesOfType.find(otherColliderID);
    if (itState == collisionStatesOfType.end())
        return CollisionState::Exit;

    return itState->second;
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
    _collisionStates[colliderType][colliderID] = stateType;
}

void CollisionComponent::RemoveCollisionState(
  ColliderType::Type colliderType, ComponentIDPair colliderID)
{
    auto it = _collisionStates.find(colliderType);

    if (_collisionStates.end() == it)
        return;

    auto& collisionStatesOfType = it->second;
    collisionStatesOfType.erase(colliderID);

    if (collisionStatesOfType.empty())
        _collisionStates.erase(it);
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
