#pragma once
#include "Animation2DSequence.h"
#include "Core/Object.h"

struct Animation2DNotify
{
    std::vector<std::function<void()>> callbacks;

    int32 frameIndex;
};

class Animation2D : public Object
{
    friend class SpriteComponent;

public:
    Animation2D()           = default;
    ~Animation2D() override = default;

private:
    std::map<std::pair<int32, int32>, Animation2DNotify> _notifies;
    Weak<class SpriteComponent>                          _owner;
    Ptr<class Animation2DSequence>                       _animationSequence;
    Ptr<class Animation2DClip>                           _currentAnimationClip;
    Ptr<class Animation2DConstantBuffer>                 _buffer;

    int32 _frameIndex = 0;
    float _accTime    = 0.f;
    float _playRate   = 0.f;
    bool  _isPlaying  = true;
    bool  _isReversed = false;
    bool  _flipX      = false;

public:
    bool Init();
    void Destroy() override;
    void Tick(float deltaTime);

    bool IsPlaying() const { return _isPlaying; }

    void SetPlayRate(float playRate);
    void SetReversed(bool isReversed);
    void SetFlipX(bool flipX);
    void SetShader();
    void Reset();

    void Play();
    void Pause();
    void Stop();

    void ChangeAnimationClip(const std::string& name, bool play = true);

private:
    void InvokeNotify();

public:
    template <typename T>
    void AddNotify(const std::string& clipName, int32 frameIndex, T* obj, void (T::*memFunc)())
    {
        Ptr<Animation2DClip> clip = _animationSequence->FindAnimationClip(clipName);
        if (nullptr == clip)
            return;

        std::pair<int32, int32> key = {clip->GetID(), frameIndex};

        auto it = _notifies.find(key);
        if (_notifies.end() == it)
            return;

        _notifies[key] = std::bind(memFunc, obj);
    }

    template <typename T>
    void AddNotify(const std::string& clipName, int32 frameIndex, T&& func)
    {
        Ptr<Animation2DClip> clip = _animationSequence->FindAnimationClip(clipName);
        if (nullptr == clip)
            return;

        std::pair<int32, int32> key = {clip->GetID(), frameIndex};

        auto it = _notifies.find(key);
        if (_notifies.end() == it)
            return;

        _notifies[key] = std::forward<T>(func);
    }
};
