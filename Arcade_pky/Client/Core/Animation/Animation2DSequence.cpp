#include "pch.h"

#include "Animation2DSequence.h"

#include "Core/ResourceManager.h"

#include "Animation2DClip.h"

void Animation2DSequence::Destroy() {}

Ptr<class Animation2DClip> Animation2DSequence::FindAnimationClip(const std::string& name)
{
    if (auto it = _animationClips.find(name); _animationClips.end() != it)
        return it->second;

    return nullptr;
}

int32 Animation2DSequence::GetAnimationClipCount() const
{
    return _animationClips.size();
}

void Animation2DSequence::AddAnimationClip(Ptr<class Animation2DClip> clip)
{
    _animationClips[clip->GetName()] = clip;
}
