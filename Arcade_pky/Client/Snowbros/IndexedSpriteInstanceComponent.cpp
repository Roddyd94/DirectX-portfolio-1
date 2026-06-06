#include "pch.h"

#include "IndexedSpriteInstanceComponent.h"

#include "Core/ResourceManager.h"

#include "IndexedAnimation2DStructureBuffer.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Texture.h"

void IndexedSpriteInstanceComponent::AddBufferData()
{
    auto clip = _animation->GetCurrentClip();
    if (nullptr == clip)
        return;

    auto renderer = GetRenderer();
    if (nullptr == renderer)
        return;

    Vector2 worldPos = GetWorldPosition().ToVector2();
    Vector2 size     = GetWorldScale().ToVector2();

    int32 frameIndex
      = _animation->IsReversed() ? GetFrameCount() - 1 - GetFrameIndex() : GetFrameIndex();
    frameIndex = std::clamp(frameIndex, 0, clip->GetFrameCount() - 1);

    Animation2DSprite                         sprite = clip->GetFrame(frameIndex);
    Ptr<IndexedTextureAnimation2DSpriteSheet> spriteSheet
      = Cast<Animation2DSpriteSheet, IndexedTextureAnimation2DSpriteSheet>(sprite.spriteSheet);

    SpriteData          data    = spriteSheet->GetSpriteData(sprite.spriteIndex);
    Ptr<IndexedTexture> texture = spriteSheet->GetTexture();
    texture->SetShaderResource();

    Vector2 uvLT;
    Vector2 uvRB;

    uvLT.x = data.start.x / texture->GetWidth();
    uvLT.y = data.start.y / texture->GetHeight();

    uvRB.x = uvLT.x + data.size.x / texture->GetWidth();
    uvRB.y = uvLT.y + data.size.y / texture->GetHeight();

    IndexedAnimation2DInstanceData instanceData;
    instanceData.uvLT          = uvLT;
    instanceData.uvRB          = uvRB;
    instanceData.worldPos      = worldPos;
    instanceData.size          = size;
    instanceData.paletteNumber = _paletteNumber;
    instanceData.flipX         = _animation->GetFlipX();

    AddData<IndexedAnimation2DStructureBuffer>(std::move(instanceData));
}

void IndexedSpriteInstanceComponent::SetPaletteNumber(int32 number)
{
    _paletteNumber = number;
}
