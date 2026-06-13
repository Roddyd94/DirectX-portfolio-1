#pragma once
#include "Animation2D.h"
#include "Core/SceneComponent.h"

class SpriteComponent : public SceneComponent
{
public:
    SpriteComponent();
    ~SpriteComponent() override;

protected:
    Ptr<class Mesh>        _mesh;
    Ptr<class Shader>      _shader;
    Ptr<class Animation2D> _animation;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;
    void Collision(float deltaTime) override;
    void Render(float deltaTime) override;

    virtual Ptr<class Animation2D> CreateAnimation();

    int32 GetFrameCount() const;
    int32 GetFrameIndex() const;
    bool GetFlipX() const;

    void SetShader(const std::string& name);
    void SetFrameIndex(int32 index);
    void SetPlayRate(float playRate);
    void SetReversed(bool isReversed);
    void SetFlipX(bool flipX);

    void Play();
    void Pause();
    void Stop();

    void ChangeAnimation(const std::string& name, bool play = true);

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
