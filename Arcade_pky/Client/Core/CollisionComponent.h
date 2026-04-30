#pragma once
#include "CollisionSystem.h"
#include "SceneComponent.h"
#include "Types.h"

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

    ComponentIDPair             _colliderID;
    Ptr<class CollisionProfile> _profile;
    std::function<void(Weak<CollisionComponent>)>
      _collisionCallBack[CollisionState::None];
    std::map<ComponentIDPair, CollisionState::Type> _collisionStates;

#if _DEBUG
    Ptr<class Mesh>                    _mesh                    = nullptr;
    Ptr<class Shader>                  _shader                  = nullptr;
    Ptr<class TransformConstantBuffer> _transformConstantBuffer = nullptr;
    Ptr<class ColorConstantBuffer>     _colorConstantBuffer     = nullptr;
#endif // _DEBUG

public:
    bool Init(int32      componentID,
      const std::string& name,
      Ptr<class Actor>   owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    CollisionShape::Type GetShape() const { return _shape; }
    CollisionState::Type CheckCollisionState(
      const ComponentIDPair& otherColliderID) const;

    Ptr<class CollisionProfile> GetProfile() const;
    const ComponentIDPair&      GetColliderID() const;

    void SetCollisionProfile(const std::string& name);

    virtual bool Collision(Weak<CollisionComponent> other);

private:
    void Invoke(
      CollisionState::Type state, Weak<CollisionComponent> otherCollider);

public:
    template <typename T>
    void SetCollisionCallBack(CollisionState::Type state,
      T*                                           obj,
      void (T::*memFunc)(Weak<CollisionComponent>))
    {
        _collisionCallBack[state]
          = std::bind(memFunc, obj, std::placeholders::_1);
    }

    template <typename T>
    void SetCollisionCallBack(CollisionState::Type state, T&& func)
    {
        _collisionCallBack[state] = std::forward<T>(func);
    }
};
