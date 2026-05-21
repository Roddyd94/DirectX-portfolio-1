#pragma once
#include "Core/Actor.h"

class Snowball : public Actor
{
public:
    Snowball()           = default;
    ~Snowball() override = default;

private:
    Ptr<class SnowballComponent> _snowballComponent;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void Destroy() override;
};
