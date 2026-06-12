#pragma once
#include "SnowbrosUIComponent.h"
#include "Core/Actor.h"

class SnowbrosUI : public Actor
{
public:
    SnowbrosUI()           = default;
    ~SnowbrosUI() override = default;

protected:
    Ptr<class SnowbrosUIComponent> _uiComponent;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;

    void SetScore(ScoreType::Type player, int32 score);
};
