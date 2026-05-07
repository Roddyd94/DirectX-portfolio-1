#pragma once
#include "Animation2DClip.h"
#include "Core/Resource.h"

class Animation2DSequence : public Resource
{
public:
    Animation2DSequence()           = default;
    ~Animation2DSequence() override = default;

private:
    std::unordered_map<std::string, Ptr<class Animation2DClip>> _animationClips;

public:
    void Destroy() override;

    Ptr<class Animation2DClip> FindAnimationClip(const std::string& name);

    int32 GetAnimationClipCount() const;

    void AddAnimationClip(Ptr<class Animation2DClip> clip);
};
