#pragma once
#include "SubManager.h"

class CameraManager : public SubManager
{
public:
    CameraManager()           = default;
    ~CameraManager() override = default;
    DELETE_SPECIAL_FUNC(CameraManager)

private:
    std::unordered_map<std::string, Ptr<class CameraComponent>> _cameras;

    Weak<class Level>          _level;
    Ptr<class CameraComponent> _mainCamera = nullptr;

    // TODO Matrix _uiProj;

public:
    bool Init(Ptr<class Level> level);
    void Destroy() override;

    Ptr<class CameraComponent> GetMainCamera() const;
    Ptr<class CameraComponent> FindCamera(const std::string& name) const;

    const Matrix&  GetViewMatrix() const;
    const Matrix&  GetProjMatrix() const;
    Vector3 GetCameraWorldPosition() const;

    void SetMainCamera(Ptr<class CameraComponent> camera);

    void ChangeMainCamera(const std::string& name);

    // TODO const Matrix& GetUIProjMatrix() const;

private:
    // TODO void CreateUIProj(float x, float y, float dist);
};
