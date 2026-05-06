#include "pch.h"

#include "SoundComponent.h"

#include "ResourceManager.h"

bool SoundComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    return SceneComponent::Init(componentID, name, owner);
}

void SoundComponent::Destroy() {}

void SoundComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void SoundComponent::SetSound(const std::string& name, bool play)
{
    _sound = SOUND_MANAGER->FindSound(name);
    if (_sound && play)
        _sound->Play();
}

void SoundComponent::SetSound(
  const std::string& name, const std::string& groupName, const char* fileName, bool loop, bool play)
{
    _sound = SOUND_MANAGER->LoadSound(name, groupName, loop, fileName);
    if (_sound && play)
        _sound->Play();
}

void SoundComponent::Play()
{
    if (_sound)
        _sound->Play();
}

void SoundComponent::Stop()
{
    if (_sound)
        _sound->Stop();
}

void SoundComponent::Pause()
{
    if (_sound)
        _sound->Pause();
}

void SoundComponent::Resume()
{
    if (_sound)
        _sound->Resume();
}
