#pragma once
#include "Core/Pawn.h"

class AI : public Pawn
{
public:
    AI()           = default;
    ~AI() override = default;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;

    Ptr<class AIController> GetController() const;
    Ptr<class AIComponent> GetAIComponent() const;
};
