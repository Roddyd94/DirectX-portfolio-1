#include "pch.h"

#include "IndexedSpriteComponent.h"

#include "Core/ResourceManager.h"

#include "IndexedAnimation2D.h"

IndexedSpriteComponent::IndexedSpriteComponent()
{
    _shouldRender = true;
}

bool IndexedSpriteComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);

    SetShader("IndexedSpriteShader");
    _mesh = MESH_TEXTURE_RECT;

    return true;
}

Ptr<class Animation2D> IndexedSpriteComponent::CreateAnimation()
{
    _animation = New<IndexedAnimation2D>();

    if (!_animation->Init())
    {
        DESTROY(_animation);
        return nullptr;
    }

    return _animation;
}

void IndexedSpriteComponent::SetPaletteNumber(int32 number)
{
    auto animation = Cast<Animation2D, IndexedAnimation2D>(_animation);
    animation->SetPaletteNumber(number);
}
