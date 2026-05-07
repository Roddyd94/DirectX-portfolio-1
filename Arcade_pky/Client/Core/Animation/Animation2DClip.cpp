#include "pch.h"

#include "Animation2DClip.h"

void Animation2DClip::Destroy()
{
    _frames.clear();
}

const Animation2DSprite& Animation2DClip::GetFrame(int32 index) const
{
    return _frames[index];
}

int32 Animation2DClip::GetFrameCount() const
{
    return _frames.size();
}

void Animation2DClip::AddFrameData(Ptr<Animation2DSpriteSheet> sprite, int32 spriteIndex)
{
    Animation2DSprite frameData;
    frameData.spriteSheet      = sprite;
    frameData.spriteIndex = spriteIndex;

    _frames.push_back(frameData);
}
