#pragma once
#include "Core/ActorComponent.h"

class SnowProjectileComponent : public ActorComponent
{
    friend class SnowProjectile;

public:
    SnowProjectileComponent()           = default;
    ~SnowProjectileComponent() override = default;

private:
    Ptr<class PlatformerKinematicComponent> _kinematic = nullptr;

    Vector2 _velocity;
    float   _direction = 1.f;
    float   _accTime   = 0.f;
    bool    _powerUp   = false;
    bool    _rangeUp   = false;

private:
    inline static float phaseTime              = 0.15f;
    inline static float initialVelocityY       = 3.f;
    inline static float velocityXPhaseNormal1  = 3.f;
    inline static float velocityXPhaseRangeUp1 = 6.f;
    inline static float velocityXPhase2        = 3.f;
    inline static float airDrag                = 2.f;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    void SetKinematic(Ptr<class PlatformerKinematicComponent> kinematic);
    void SetDirection(float direction);
};
