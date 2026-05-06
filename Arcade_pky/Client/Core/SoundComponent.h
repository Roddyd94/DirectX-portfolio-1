#pragma once
#include "SceneComponent.h"

class SoundComponent : public SceneComponent
{
public:
    SoundComponent()           = default;
    ~SoundComponent() override = default;

protected:
    Ptr<class Sound> _sound;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;

    void Tick(float deltaTime) override;

    void SetSound(const std::string& name, bool play);
    void SetSound(const std::string& name,
      const std::string&             groupName,
      const char*                    fileName,
      bool                           loop,
      bool                           play);

    void Play();
    void Stop();
    void Pause();
    void Resume();
};
