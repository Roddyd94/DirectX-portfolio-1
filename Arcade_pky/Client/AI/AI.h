#pragma once
#include "Core/Pawn.h"

class AI : public Pawn
{
public:
    AI()           = default;
    ~AI() override = default;

protected:
    Ptr<class AIComponent>  _aiComponent;
    Ptr<class AIController> _aiController;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;

    Ptr<class AIController> GetController() const;
};
