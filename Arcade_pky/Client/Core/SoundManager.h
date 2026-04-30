#pragma once
#include "SubManager.h"

#include "Sound.h"

class SoundManager : public SubManager
{
public:
    SoundManager()          = default;
    virtual ~SoundManager() = default;

private:
    FMOD::System*                                        _system;
    FMOD::ChannelGroup*                                  _masterGroup;
    std::unordered_map<std::string, Ptr<Sound>>          _sounds;
    std::unordered_map<std::string, FMOD::ChannelGroup*> _channelGroups;

public:
    bool Init() override;
    void Destroy() override;
    void Tick();

    Ptr<Sound> FindSound(const std::string& name);

    FMOD::ChannelGroup* CreateChannelGroup(const std::string& name);
    Ptr<Sound>          LoadSound(const std::string& name,
               const std::string&                    groupName,
               bool                                  loop,
               const char*                           fileName);
    void                SetMasterVolume(float volume);
    void SetGroupVolume(const std::string& groupName, float volume);

    void Play(const std::string& name);
    void Stop(const std::string& name);
    void Pause(const std::string& name);
    void Resume(const std::string& name);

private:
    FMOD::ChannelGroup* FindChannelGroup(const std::string& groupName);
};
