#pragma once
#include "Core/Animation/Animation2D.h"
#include "Core/RenderInstanceComponent.h"

class SpriteInstanceComponent : public RenderInstanceComponent
{
public:
    SpriteInstanceComponent();
    ~SpriteInstanceComponent() override = default;

protected:
    Ptr<class Animation2D> _animation;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void PreRender(float deltaTime) override;

    void AddBufferData() override;

    int32 GetFrameCount() const;
    int32 GetFrameIndex() const;
    bool  GetFlipX() const;

    void SetFrameIndex(int32 index);
    void SetPlayRate(float playRate);
    void SetReversed(bool isReversed);
    void SetFlipX(bool flipX);

    void Play();
    void Pause();
    void Stop();

    void ChangeAnimation(const std::string& name, bool play = true);

    Ptr<class Animation2D> CreateAnimation();
    Ptr<class Animation2D> GetAnimation();
    const std::string&     GetCurrentClipName() const;

public:
    template <typename T>
    void AddNotify(const std::string& clipName, int32 frameIndex, T* obj, void (T::*memFunc)())
    {
        _animation->AddNotify(clipName, frameIndex, obj, memFunc);
    }

    template <typename T>
    void AddNotify(const std::string& clipName, int32 frameIndex, T&& func)
    {
        _animation->AddNotify(clipName, frameIndex, std::forward<T>(func));
    }
};
