#pragma once
#include "Core/Actor.h"

class SnowbrosCheat : public Actor
{
public:
    SnowbrosCheat()           = default;
    ~SnowbrosCheat() override = default;
public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void ChangePalette(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
};
