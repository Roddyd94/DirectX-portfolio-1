#pragma once
#include "Core/ActorComponent.h"
#include "Core/Input/Types.h"

class ShootComponent : public ActorComponent
{
public:
    ShootComponent()           = default;
    ~ShootComponent() override = default;

private:
    float _extentX  = 0.f;
    float _fireRate = 0.15f;
    float _accTime  = 0.15f;
    bool  _powerUp  = false;
    bool  _rangeUp  = false;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime);

    void HandleInput(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);

    void Shoot();
    void PowerUp();
    void RangeUp();
    void Reset();

    void SetExtentX(float extent);
    void SetFireRate(float fireRate);
};
