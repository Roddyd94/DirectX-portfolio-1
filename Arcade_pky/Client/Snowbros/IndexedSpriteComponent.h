#pragma once
#include "Core/Animation/SpriteComponent.h"

class IndexedSpriteComponent : public SpriteComponent
{
public:
    IndexedSpriteComponent();
    ~IndexedSpriteComponent() override = default;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    
    Ptr<class Animation2D> CreateAnimation() override;

    void SetPaletteNumber(int32 number);
};
