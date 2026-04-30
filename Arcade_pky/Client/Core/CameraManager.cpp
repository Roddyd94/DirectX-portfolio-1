#include "pch.h"

#include "CameraManager.h"

#include "DeviceManager.h"

#include "CameraComponent.h"
#include "Info.h"

bool CameraManager::Init(Ptr<class Level> level)
{
    _level = level;

    Resolution rs = DeviceManager::Instance().GetResolution();

    // TODO CreateUIProj(static_cast<float>(rs._width),
    // static_cast<float>(rs._height), 2000);

    return false;
}

void CameraManager::Destroy() {}

Ptr<class CameraComponent> CameraManager::GetMainCamera() const
{
    return _mainCamera;
}

Ptr<class CameraComponent> CameraManager::FindCamera(
  const std::string& name) const
{
    if (auto it = _cameras.find(name); it != _cameras.end())
        return it->second;

    return nullptr;
}

const Matrix& CameraManager::GetViewMatrix() const
{
    return _mainCamera->GetViewMatrix();
}

const Matrix& CameraManager::GetProjMatrix() const
{
    return _mainCamera->GetProjMatrix();
}

Vector3 CameraManager::GetCameraWorldPosition() const
{
    return _mainCamera->GetWorldPosition();
}

void CameraManager::SetMainCamera(Ptr<CameraComponent> camera)
{
    _mainCamera                      = camera;
    _cameras[_mainCamera->GetName()] = _mainCamera;
}

void CameraManager::ChangeMainCamera(const std::string& name)
{
    auto foundCamera = FindCamera(name);
    if (!foundCamera)
        return;

    _mainCamera = foundCamera;
}
