#include "pch.h"

#include "IndexedSpriteInstanceRendererComponent.h"

#include "Core/ResourceManager.h"

#include "Core/PaletteStructureBuffer.h"
#include "Core/Texture.h"

IndexedSpriteInstanceRendererComponent::IndexedSpriteInstanceRendererComponent()
{
    _shouldRender = true;
}

bool IndexedSpriteInstanceRendererComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    InstanceRendererComponent::Init(componentID, name, owner);

    _paletteBuffer = FIND_STRUCTURE_BUFFER("Palette", PaletteStructureBuffer);

    return true;
}

void IndexedSpriteInstanceRendererComponent::Render(float deltaTime)
{
    if (!_texture)
        return;

    _paletteBuffer->Update();
    _paletteBuffer->Bind();
    _texture->SetShaderResource();

    InstanceRendererComponent::Render(deltaTime);
}

void IndexedSpriteInstanceRendererComponent::SetTexture(Ptr<class IndexedTexture> texture)
{
    _texture = texture;
}
