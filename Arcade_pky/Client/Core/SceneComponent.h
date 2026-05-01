#pragma once
#include "Component.h"
#include "Info.h"
#include "Transform.h"
#include "TransformMatrix.h"

class SceneComponent : public Component
{
    friend class Actor;

public:
    SceneComponent();
    virtual ~SceneComponent();

protected:
    TransformMatrix _matrix;
    Transform       _world;
    Transform       _relative;

    std::string                                _renderLayerName;
    std::map<int32, Ptr<class SceneComponent>> _children;

    Weak<class TransformConstantBuffer> _transformConstantBuffer;
    Weak<class SceneComponent>          _parent;

    Vector3 _axis[AxisType::End] = {
      Vector3(Vector3::axisX),
      Vector3(Vector3::axisY),
      Vector3(Vector3::axisZ),
    };

    bool _isRendering = false;

public:
    bool Init(int32      componentID,
      const std::string& name,
      Ptr<class Actor>   owner) override;
    void Destroy() override;
    void Tick(float deltaTime) override;

    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);

    virtual void AttachToComponent(Ptr<SceneComponent> comp);

    Ptr<SceneComponent> GetParent() const;
    Ptr<SceneComponent> FindComponent(int32 componentID) const;
    const std::string&  GetRenderLayerName() const;

    const Transform& GetWorldTransform() const;
    Vector3          GetWorldScale() const;
    Vector3          GetWorldPosition() const;
    Vector3          GetWorldRotation() const;

    const Transform& GetRelativeTransform() const;
    Vector3          GetRelativeScale() const;
    Vector3          GetRelativePosition() const;
    Vector3          GetRelativeRotation() const;

    void AddChild(Ptr<SceneComponent> comp);
    void SetRenderLayer(const std::string& name);

    void SetWorldTransform(const Transform& transform);
    void SetWorldTransform(Vector3 position, Vector3 scale, Vector3 rotation);
    void SetWorldScale(Vector3 scale);
    void SetWorldPosition(Vector3 position);
    void SetWorldRotation(Vector3 rotation);

    void SetRelativeTransform(const Transform& transform);
    void SetRelativeTransform(
      Vector3 position, Vector3 scale, Vector3 rotation);
    void SetRelativeScale(Vector3 scale);
    void SetRelativePosition(Vector3 position);
    void SetRelativeRotation(Vector3 rotation);

private:
    void UpdateTransform();
};
