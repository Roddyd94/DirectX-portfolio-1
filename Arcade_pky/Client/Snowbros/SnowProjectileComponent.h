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

    float _direction = 1.f;
    float _accTime   = 0.f;

    int32 _playerNumber = 0;

    bool _powerUp = false;
    bool _rangeUp = false;
    bool _setHit  = false;
    bool _isHit   = false;

private:
    inline static float phaseTime              = 0.15f;
    inline static float lifeTime               = 0.8f;
    inline static float initialVelocityY       = 5.f;
    inline static float velocityXPhase1Normal  = 12.f;
    inline static float velocityXPhase1RangeUp = 24.f;
    inline static float velocityXPhase2        = 4.f;
    inline static float airDrag                = 6.f;
    inline static float gravityMultiplier      = 3.f;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    void OnCollisionWith(Weak<class CollisionComponent> other);
    void OnCollision();

    bool IsHit() const;
    bool IsPoweredUp() const;

    int32 GetPlayerNumber() const;

    void SetPlayer(int32 number);
    void SetKinematic(Ptr<class PlatformerKinematicComponent> kinematic);
    void SetDirection(float direction);
};
