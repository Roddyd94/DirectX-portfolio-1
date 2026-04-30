#include "pch.h"

#include "SoundManager.h"

bool SoundManager::Init()
{
    FMOD_RESULT result = FMOD::System_Create(&_system);
    if (result != FMOD_OK)
        return false;

    result = _system->init(256, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK)
        return false;

    result = _system->getMasterChannelGroup(&_masterGroup);
    if (result != FMOD_OK)
        return false;

    _channelGroups["Master"] = _masterGroup;

    CreateChannelGroup("BGM");
    CreateChannelGroup("SFX");

    // TODO: Load Metadata and Read Files
    float volumeBGM = 80.0f; // TODO: read from configs
    float volumeSFX = 80.0f; // TODO: read from configs

    SetGroupVolume("BGM", 80.f);
    SetGroupVolume("SFX", 80.f);

    return true;
}

void SoundManager::Destroy()
{
    for (auto& [_, sound] : _sounds)
        DESTROY(sound);

    for (auto [_, group] : _channelGroups)
        group->release();

    if (_system)
    {
        _system->release();
        _system->close();
    }

    _sounds.clear();
    _channelGroups.clear();
    _system = nullptr;
}

void SoundManager::Tick()
{
    _system->update();
}

Ptr<Sound> SoundManager::FindSound(const std::string& name)
{
    if (auto it = _sounds.find(name); _sounds.end() != it)
        return it->second;

    return nullptr;
}

FMOD::ChannelGroup* SoundManager::CreateChannelGroup(const std::string& name)
{
    FMOD::ChannelGroup* groupFound = FindChannelGroup(name);
    if (nullptr != groupFound)
        return groupFound;

    FMOD::ChannelGroup* group = nullptr;
    FMOD_RESULT result = _system->createChannelGroup(name.c_str(), &group);
    if (result != FMOD_OK)
        return nullptr;

    _masterGroup->addGroup(group);
    _channelGroups[name] = group;

    return group;
}

Ptr<Sound> SoundManager::LoadSound(const std::string& name,
  const std::string&                                  groupName,
  bool                                                loop,
  const char*                                         fileName)
{
    FMOD::ChannelGroup* group = FindChannelGroup(groupName);
    if (nullptr == group)
        return nullptr;

    Ptr<Sound> sound = FindSound(name);
    if (nullptr != sound)
        return sound;

    sound = New<Sound>();
    if (!sound->LoadSound(_system, group, loop, fileName))
    {
        DESTROY(sound);
        return nullptr;
    }

    _sounds[name] = sound;
    return sound;
}

void SoundManager::SetMasterVolume(float volume)
{
    volume = std::clamp(volume, 0.f, 100.f);
    _masterGroup->setVolume(volume / 100.f);
}

void SoundManager::SetGroupVolume(const std::string& groupName, float volume)
{
    FMOD::ChannelGroup* group = FindChannelGroup(groupName);
    if (nullptr == group)
        return;

    volume = std::clamp(volume, 0.f, 100.f);
    group->setVolume(volume / 100.f);
}

void SoundManager::Play(const std::string& name)
{
    Ptr<Sound> sound = FindSound(name);
    if (nullptr == sound)
        return;

    sound->Play();
}

void SoundManager::Stop(const std::string& name)
{
    Ptr<Sound> sound = FindSound(name);
    if (nullptr == sound)
        return;

    sound->Stop();
}

void SoundManager::Pause(const std::string& name)
{
    Ptr<Sound> sound = FindSound(name);
    if (nullptr == sound)
        return;

    sound->Pause();
}

void SoundManager::Resume(const std::string& name)
{
    Ptr<Sound> sound = FindSound(name);
    if (nullptr == sound)
        return;

    sound->Resume();
}

FMOD::ChannelGroup* SoundManager::FindChannelGroup(const std::string& groupName)
{
    if (auto it = _channelGroups.find(groupName); _channelGroups.end() != it)
        return it->second;

    return nullptr;
}
