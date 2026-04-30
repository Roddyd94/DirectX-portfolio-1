#pragma once
#include "Core/Actor.h"

class TestActor : public Actor
{
public:
    bool Init(int32  id,
      Vector3 position,
      Vector3 scale,
      Vector3 rotation) override;
};
