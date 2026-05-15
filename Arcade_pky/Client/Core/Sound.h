#pragma once
#include "Resource.h"
#include "FMOD/fmod.hpp"

class Sound : public Resource
{
public:
    Sound()           = default;
    ~Sound() override = default;

protected:
    FMOD::System*       _system  = nullptr;
    FMOD::ChannelGroup* _group   = nullptr;
    FMOD::Channel*      _channel = nullptr;
    FMOD::Sound*        _sound   = nullptr;

    int32 _playCount = 0;
    bool  _loop      = false;

public:
    void Destroy() override;

    bool LoadSound(FMOD::System* system,
      FMOD::ChannelGroup*        group,
      bool                       loop,
      const char*                fileName);

    void Play();
    void Stop();
    void Pause();
    void Resume();

public:
    static FMOD_RESULT SoundCallback(FMOD_CHANNELCONTROL* channelcontrol,
      FMOD_CHANNELCONTROL_TYPE                            controltype,
      FMOD_CHANNELCONTROL_CALLBACK_TYPE                   callbacktype,
      void*                                               commanddata1,
      void*                                               commanddata2);
};
