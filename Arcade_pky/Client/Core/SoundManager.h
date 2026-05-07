#pragma once
#include "SubManager.h"

#include "Sound.h"

class SoundManager : public SubManager
{
public:
    SoundManager()           = default;
    ~SoundManager() override = default;
    DELETE_SPECIAL_FUNC(SoundManager)

private:
    std::unordered_map<std::string, FMOD::ChannelGroup*> _channelGroups;
    std::unordered_map<std::string, int32>               _soundFinder;
    std::map<int32, Ptr<Sound>>                          _sounds;

    FMOD::System*       _system;
    FMOD::ChannelGroup* _masterGroup;

    int32 _idCounter = 0;

public:
    bool Init() override;
    void Destroy() override;
    void Tick();

    Ptr<Sound> FindSound(const std::string& name);

    FMOD::ChannelGroup* CreateChannelGroup(const std::string& name);
    Ptr<Sound>          LoadSound(
               const std::string& name, const std::string& groupName, bool loop, const char* fileName);
    void SetMasterVolume(float volume);
    void SetGroupVolume(const std::string& groupName, float volume);

    void Play(const std::string& name);
    void Stop(const std::string& name);
    void Pause(const std::string& name);
    void Resume(const std::string& name);

private:
    FMOD::ChannelGroup* FindChannelGroup(const std::string& groupName);
};
