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
    _isRendering     = true;
    _renderLayerName = "Collider";
#endif // _DEBUG
}

CollisionComponent::~CollisionComponent() {}

bool CollisionComponent::Init(
  int32 componentID, const std::string& name, Ptr<Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    _colliderID = {owner->GetActorID(), GetComponentID()};

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return false;

    level->AddCollision(_colliderID, This<CollisionComponent>());

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

    level->RemoveCollision(_colliderID);

    for (auto& [colliderID, collisionState] : _collisionStates)
    {
        Ptr<CollisionComponent> foundCollider = level->FindCollider(colliderID);
        if (nullptr == foundCollider)
            continue;

        foundCollider->_collisionStates.erase(_colliderID);
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
  const ComponentIDPair& otherColliderID) const
{
    auto it = _collisionStates.find(otherColliderID);
    if (it == _collisionStates.end())
        return CollisionState::Exit;

    return it->second;
}

Ptr<CollisionProfile> CollisionComponent::GetProfile() const
{
    return _profile;
}

const ComponentIDPair& CollisionComponent::GetColliderID() const
{
    return _colliderID;
}

void CollisionComponent::SetCollisionProfile(const std::string& name)
{
    Ptr<Level> level = GetLevel();

    Ptr<CollisionProfile> profileFound
      = level->GetCollisionProfileManager()->FindProfile(name);
    if (nullptr == profileFound)
        return;

    _profile = profileFound;
}

bool CollisionComponent::Collision(Weak<CollisionComponent> other)
{
    return false;
}

void CollisionComponent::Invoke(
  CollisionState::Type state, Weak<CollisionComponent> other)
{
    Ptr<CollisionComponent> otherCollider = Lock<CollisionComponent>(other);
    if (nullptr == otherCollider)
        return;

    ComponentIDPair otherColliderID = otherCollider->GetColliderID();
    if (state == CollisionState::Enter || state == CollisionState::Stay)
        _collisionStates.emplace(std::make_pair(otherColliderID, state));
    else
        _collisionStates.erase(otherColliderID);

    if (_collisionCallBack[state])
        _collisionCallBack[state](otherCollider);
}
