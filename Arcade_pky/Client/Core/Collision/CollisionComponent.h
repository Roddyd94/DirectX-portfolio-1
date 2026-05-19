#pragma once
#include "CollisionSystem.h"
#include "Core/SceneComponent.h"
#include "Core/Types.h"

class CollisionComponent : public SceneComponent
{
    friend class CollisionManager;

public:
    CollisionComponent();
    ~CollisionComponent() override;

protected:
    CollisionShape::Type _shape = CollisionShape::End;
    Vector3              _min;
    Vector3              _max;

    ComponentIDPair                               _colliderID;
    Ptr<class CollisionProfile>                   _profile;
    std::function<void(Weak<CollisionComponent>)> _collisionCallback[CollisionState::None];
    std::map<ColliderType::Type, std::map<ComponentIDPair, CollisionState::Type>> _collisionStates;

#if _DEBUG
    Ptr<class Mesh>                    _mesh                    = nullptr;
    Ptr<class Shader>                  _shader                  = nullptr;
    Ptr<class TransformConstantBuffer> _transformConstantBuffer = nullptr;
    Ptr<class ColorConstantBuffer>     _colorConstantBuffer     = nullptr;
#endif // _DEBUG

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    CollisionShape::Type GetShape() const { return _shape; }
    CollisionState::Type CheckCollisionState(Ptr<class CollisionComponent> otherCollider) const;
    CollisionState::Type CheckCollisionState(ColliderType::Type otherColliderType, ComponentIDPair otherColliderID) const;

    Ptr<class CollisionProfile> GetProfile() const;
    ColliderType::Type          GetColliderType() const;
    ComponentIDPair             GetColliderID() const;

    void SetCollisionProfile(const std::string& name);
    void AddCollisionState(
      ColliderType::Type colliderType, ComponentIDPair colliderID, CollisionState::Type stateType);
    void RemoveCollisionState(ColliderType::Type colliderType, ComponentIDPair colliderID);

    virtual bool Collision(Weak<CollisionComponent> other);

private:
    void Invoke(CollisionState::Type state, Weak<CollisionComponent> otherCollider);

public:
    template <typename T>
    void RegisterCollisionCallBack(
      CollisionState::Type state, T* obj, void (T::*memFunc)(Weak<CollisionComponent>))
    {
        _collisionCallback[state] = std::bind(memFunc, obj, std::placeholders::_1);
    }

    template <typename T>
    void RegisterCollisionCallBack(CollisionState::Type state, T&& func)
    {
        _collisionCallback[state] = std::forward<T>(func);
    }
};
