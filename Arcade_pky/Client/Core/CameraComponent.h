#pragma once
#include "SceneComponent.h"

enum class CameraType
{
    Perspective,
    Orthographic
};

class CameraComponent : public SceneComponent
{
public:
    CameraComponent();
    ~CameraComponent() override;

protected:
    Matrix     _matView      = {};
    Matrix     _matProj      = {};
    CameraType _type         = CameraType::Orthographic;
    float      _width        = 0.f;
    float      _height       = 0.f;
    float      _viewAngle    = 90.f;
    float      _viewDistance = 1000.f;

public:
    bool Init(int32      componentID,
      const std::string& name,
      Ptr<class Actor>   owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    float GetWidth() const { return _width; }
    float GetHeight() const { return _height; }
    float GetViewAngle() const { return _viewAngle; }
    float GetViewDistance() const { return _viewDistance; }

    CameraType GetProjectionType() const { return _type; }

    const Matrix& GetViewMatrix() const { return _matView; }
    const Matrix& GetProjMatrix() const { return _matProj; }

    void SetWidth(float width) { _width = width; }
    void SetHeight(float height) { _height = height; }
    void SetViewResolution(float width, float height);
    void SetViewDistance(float distance) { _viewDistance = distance; }
    void SetViewAngle(float angle) { _viewAngle = angle; }

    void SetProjectionType(CameraType type);
};
