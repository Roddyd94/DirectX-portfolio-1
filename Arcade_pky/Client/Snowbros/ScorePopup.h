#pragma once
#include "Core/Actor.h"
class ScorePopup : public Actor
{
public:
    enum Type
    {
        General,
        Sushi,
        Envelope,
        End
    };

public:
    ScorePopup()           = default;
    ~ScorePopup() override = default;

protected:
    Ptr<class SpriteInstanceComponent> _spriteInstance;

    float _velocity = 1.f;
    int32 _timerID  = -1;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    void SetInfo(ScorePopup::Type scoreType, int32 index);
};
