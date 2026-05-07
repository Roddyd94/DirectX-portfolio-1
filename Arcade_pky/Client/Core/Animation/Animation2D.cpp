#include "pch.h"

#include "Animation2D.h"

#include "Animation2DConstantBuffer.h"
#include "Core/Texture.h"

bool Animation2D::Init()
{
    return false;
}

void Animation2D::Destroy()
{
    _notifies.clear();
}

void Animation2D::Tick(float deltaTime)
{
    if (!_isPlaying)
        return;

    _accTime += deltaTime * _playRate;

    if (_accTime < _currentAnimationClip->GetFrameTime())
        return;

    _accTime -= _currentAnimationClip->GetFrameTime();

    if (_isReversed)
    {
        --_frameIndex;

        if (_frameIndex < 0)
        {
            if (_currentAnimationClip->IsLoop())
            {
                _frameIndex = _currentAnimationClip->GetFrameCount() - 1;
            }
            else
            {
                _frameIndex = 0;
                _isPlaying  = false;
            }
        }
    }
    else
    {
        ++_frameIndex;

        if (_frameIndex >= _currentAnimationClip->GetFrameCount())
        {
            if (_currentAnimationClip->IsLoop())
                _frameIndex = 0;
            else
            {
                _frameIndex = _currentAnimationClip->GetFrameCount() - 1;
                _isPlaying  = false;
            }
        }
    }

    if (_isPlaying)
        InvokeNotify();
}

void Animation2D::SetPlayRate(float playRate)
{
    _playRate = playRate;
}

void Animation2D::SetReversed(bool isReversed)
{
    _isReversed = isReversed;
}

void Animation2D::SetFlipX(bool flipX)
{
    _flipX = flipX;
}

void Animation2D::SetShader()
{
    Animation2DSprite sprite = _currentAnimationClip->GetFrame(_frameIndex);
    SpriteData        data   = sprite.spriteSheet->GetSpriteData(sprite.spriteIndex);

    Ptr<Texture> texture = sprite.spriteSheet->GetTexture();

    Vector2 uvLT;
    Vector2 uvRB;

    uvLT.x = data.start.x / texture->GetWidth();
    uvLT.y = data.start.y / texture->GetHeight();

    uvRB.x = uvLT.x + data.size.x / texture->GetWidth();
    uvRB.y = uvLT.y + data.size.y / texture->GetHeight();

    _buffer->SetUV(uvLT, uvRB);
    _buffer->SetFlipX(_flipX);
    _buffer->Update();
}

void Animation2D::Reset()
{
    _frameIndex = 0;
    _accTime    = 0.f;
}

void Animation2D::Play()
{
    _isPlaying = true;
}

void Animation2D::Pause()
{
    _isPlaying = false;
}

void Animation2D::Stop()
{
    _isPlaying = false;
    Reset();
}

void Animation2D::ChangeAnimationClip(const std::string& name, bool play)
{
    Ptr<Animation2DClip> clip = _animationSequence->FindAnimationClip(name);
    if (nullptr == clip)
        return;

    _currentAnimationClip = clip;
    Reset();
    _isPlaying = play;
}

void Animation2D::InvokeNotify()
{
    std::pair<int32, int32> key = {_currentAnimationClip->GetID(), _frameIndex};

    auto it = _notifies.find(key);
    if (_notifies.end() == it)
        return;

    Animation2DNotify& notify = it->second;

    size_t size = notify.callbacks.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (notify.callbacks[i])
            notify.callbacks[i]();
    }
}
