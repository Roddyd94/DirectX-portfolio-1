#pragma once
#include "Core/InstanceRendererComponent.h"

class IndexedSpriteInstanceRendererComponent : public InstanceRendererComponent
{
public:
    IndexedSpriteInstanceRendererComponent();
    ~IndexedSpriteInstanceRendererComponent() override = default;

protected:
    Ptr<class StructureBuffer> _paletteBuffer = nullptr;
    Ptr<class IndexedTexture>  _texture       = nullptr;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Render(float deltaTime) override;

    void SetTexture(Ptr<class IndexedTexture> texture);
};
