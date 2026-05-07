#pragma once
#include "Animation2DSprite.h"
#include "Core/Resource.h"

class Animation2DClip : public Resource
{
public:
    Animation2DClip()           = default;
    ~Animation2DClip() override = default;

private:
    std::vector<Animation2DSprite> _frames;

    float _frameTime = 0.f;
    bool  _isLoop    = false;

public:
    void Destroy() override;

    const Animation2DSprite& GetFrame(int32 index) const;

    float GetFrameTime() const { return _frameTime; }
    float GetPlayTime() const { return _frameTime * GetFrameCount(); }
    bool  IsLoop() const { return _isLoop; }

    int32 GetFrameCount() const;

    void SetFrameTime(float frameTime) { _frameTime = frameTime; }
    void SetLoop(bool isLoop) { _isLoop = isLoop; }

    void AddFrameData(Ptr<class Animation2DSpriteSheet> sprite, int32 spriteIndex);
};
