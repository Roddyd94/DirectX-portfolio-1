#include "pch.h"

#include "SpriteInstanceComponent.h"

#include "Core/ResourceManager.h"

#include "Animation2DStructureBuffer.h"
#include "Core/Animation/Animation2D.h"
#include "Core/Texture.h"

SpriteInstanceComponent::SpriteInstanceComponent()
{
    _shouldRender = true;
}

bool SpriteInstanceComponent::Init(
  int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    RenderInstanceComponent::Init(componentID, name, owner);

    return true;
}

void SpriteInstanceComponent::Destroy()
{
    RenderInstanceComponent::Destroy();
    DESTROY(_animation);
}

void SpriteInstanceComponent::Tick(float deltaTime)
{
    RenderInstanceComponent::Tick(deltaTime);

    if (_animation)
        _animation->Tick(deltaTime);
}

void SpriteInstanceComponent::PreRender(float deltaTime)
{
    auto renderer = GetRenderer();

    if (nullptr == renderer)
        return;

    AddBufferData();
}

void SpriteInstanceComponent::AddBufferData()
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

    Animation2DSprite                  sprite = clip->GetFrame(frameIndex);
    Ptr<TextureAnimation2DSpriteSheet> spriteSheet
      = Cast<Animation2DSpriteSheet, TextureAnimation2DSpriteSheet>(sprite.spriteSheet);

    SpriteData   data    = spriteSheet->GetSpriteData(sprite.spriteIndex);
    Ptr<Texture> texture = spriteSheet->GetTexture();
    texture->SetShaderResource(0, ShaderType::Pixel, 0);

    Vector2 uvLT;
    Vector2 uvRB;

    uvLT.x = data.start.x / texture->GetWidth();
    uvLT.y = data.start.y / texture->GetHeight();

    uvRB.x = uvLT.x + data.size.x / texture->GetWidth();
    uvRB.y = uvLT.y + data.size.y / texture->GetHeight();

    Animation2DInstanceData instanceData;
    instanceData.uvLT     = uvLT;
    instanceData.uvRB     = uvRB;
    instanceData.worldPos = worldPos;
    instanceData.size     = size;
    instanceData.flipX    = _animation->GetFlipX();

    renderer->AddData<Animation2DStructureBuffer>(std::move(instanceData));
}

int32 SpriteInstanceComponent::GetFrameCount() const
{
    return _animation->GetFrameCount();
}

int32 SpriteInstanceComponent::GetFrameIndex() const
{
    return _animation->GetFrameIndex();
}

bool SpriteInstanceComponent::GetFlipX() const
{
    return _animation->GetFlipX();
}

void SpriteInstanceComponent::SetFrameIndex(int32 index)
{
    _animation->SetFrameIndex(index);
}

void SpriteInstanceComponent::SetPlayRate(float playRate)
{
    _animation->SetPlayRate(playRate);
}

void SpriteInstanceComponent::SetReversed(bool isReversed)
{
    _animation->SetReversed(isReversed);
}

void SpriteInstanceComponent::SetFlipX(bool flipX)
{
    _animation->SetFlipX(flipX);
}

void SpriteInstanceComponent::Play()
{
    _animation->Play();
}

void SpriteInstanceComponent::Pause()
{
    _animation->Pause();
}

void SpriteInstanceComponent::Stop()
{
    _animation->Stop();
}

void SpriteInstanceComponent::ChangeAnimation(const std::string& name, bool play)
{
    if (GetCurrentClipName() != name)
        _animation->ChangeAnimationClip(name, play);
}

Ptr<class Animation2D> SpriteInstanceComponent::CreateAnimation()
{
    _animation = New<Animation2D>();

    if (!_animation->Init())
    {
        DESTROY(_animation);
        return nullptr;
    }

    return _animation;
}

Ptr<class Animation2D> SpriteInstanceComponent::GetAnimation()
{
    return _animation;
}

const std::string& SpriteInstanceComponent::GetCurrentClipName() const
{
    return _animation->GetCurrentClip()->GetName();
}
