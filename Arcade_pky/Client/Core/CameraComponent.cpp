#include "pch.h"

#include "CameraComponent.h"

#include "DeviceManager.h"

#include "Actor.h"
#include "Level.h"

CameraComponent::CameraComponent()
{
    Resolution resolution = DeviceManager::Instance().GetResolution();

    _width  = defaultViewVolume.width;
    _height = defaultViewVolume.height;
}

CameraComponent::~CameraComponent() {}

bool CameraComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    SetProjectionType(_type);

    Ptr<Level> level = owner->GetLevel();
    if (nullptr == level)
        return false;

    level->SetMainCamera(This<CameraComponent>());

    return true;
}

void CameraComponent::Destroy()
{
    Ptr<Level> level = GetOwner()->GetLevel();
    level->RemoveCamera(This<CameraComponent>());
}

void CameraComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);

    _matView.ToIdentity();

    for (int i = 0; i < AxisType::End; ++i)
    {
        Vector3 axis = _axis[i];
        memcpy(&_matView[i][0], &axis, sizeof(Vector3));
    }

    _matView.Transpose();

    _matView._41 = -_axis[AxisType::X].Dot(_world.position);
    _matView._42 = -_axis[AxisType::Y].Dot(_world.position);
    _matView._43 = -_axis[AxisType::Z].Dot(_world.position);
}

void CameraComponent::Collision(float deltaTime)
{
    SceneComponent::Collision(deltaTime);
}

void CameraComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);
}

void CameraComponent::SetViewResolution(float width, float height)
{
    _width  = width;
    _height = height;
}

void CameraComponent::SetProjectionType(CameraType type)
{
    _type = type;

    switch (_type)
    {
    case CameraType::Perspective:
    {
        float fovRadian = DirectX::XMConvertToRadians(_viewAngle);
        _matProj = DirectX::XMMatrixPerspectiveFovLH(fovRadian, _width / _height, 0.1f, 1000.f);
    }
    break;
    case CameraType::Orthographic:
        _matProj = DirectX::XMMatrixOrthographicOffCenterLH(
          _width / -2, _width / 2, _height / -2, _height / 2, 0.f, _viewDistance);
        break;
    default:
        break;
    }
}
