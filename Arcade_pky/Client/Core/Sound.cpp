#include "pch.h"

#include "Sound.h"

#include "DirectoryManager.h"

#pragma comment(lib, "fmod_vc.lib")

void Sound::Destroy()
{
    if (_sound)
        _sound->release();

    _sound = nullptr;
}

bool Sound::LoadSound(FMOD::System* system,
  FMOD::ChannelGroup*               group,
  bool                              loop,
  const char*                       fileName)
{
    _system = system;
    _group  = group;
    _loop   = loop;

    auto resPath = DirectoryManager::Instance().GetCachePath("Resources");
    if (!resPath.has_value())
        return false;

    std::filesystem::path fullPath;
    if (!DirectoryManager::Instance().GetFile(
          resPath.value(), fileName, OUT fullPath))
        return false;

    FMOD_MODE mode = _loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;

    if (_system->createSound(fullPath.string().c_str(), mode, nullptr, &_sound)
        != FMOD_OK)
        return false;

    return true;
}

void Sound::Play()
{
    _system->playSound(_sound, _group, false, &_channel);
    ++_playCount;

    _channel->setCallback(Sound::SoundCallBack);
}

void Sound::Stop()
{
    if (_channel)
    {
        --_playCount;

        bool playing = false;
        _channel->isPlaying(&playing);

        if (playing)
        {
            _channel->stop();

            if (_playCount == 0)
                _channel = nullptr;
        }
    }
}

void Sound::Pause()
{
    if (_channel)
    {
        bool playing = false;
        _channel->isPlaying(&playing);

        if (playing)
            _channel->setPaused(true);
    }
}

void Sound::Resume()
{
    if (_channel)
    {
        bool paused = false;
        _channel->getPaused(&paused);

        if (paused)
            _channel->setPaused(false);
    }
}

FMOD_RESULT Sound::SoundCallBack(FMOD_CHANNELCONTROL* channelcontrol,
  FMOD_CHANNELCONTROL_TYPE                            controltype,
  FMOD_CHANNELCONTROL_CALLBACK_TYPE                   callbacktype,
  void*                                               commanddata1,
  void*                                               commanddata2)
{
    return FMOD_OK;
}
