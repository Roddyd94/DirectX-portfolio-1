#include "pch.h"

#include "Animation2DManager.h"

#include "Core/ResourceManager.h"

#include "Animation2DClip.h"
#include "Animation2DSequence.h"
#include "Animation2DSprite.h"

bool Animation2DManager::Init()
{
    // TODO read files and create animation data
    CreateSpriteSheet("Player", "Player", L"snowbros_player.png");
    CreateSpriteSheet("Enemy", "Enemy", L"snowbros_enemies.png");

    return true;
}

void Animation2DManager::Destroy()
{
    for (auto& [_, sheet] : _spriteSheets)
        DESTROY(sheet)

    for (auto& [_, clip] : _animationClips)
        DESTROY(clip)

    _spriteSheets.clear();
    _animationClips.clear();
}

Ptr<Animation2DSpriteSheet> Animation2DManager::FindSpriteSheet(const std::string& name)
{
    auto itFinder = _spriteSheetFinder.find(name);
    if (_spriteSheetFinder.end() == itFinder)
        return nullptr;

    int32 id      = itFinder->second;
    auto  itSheet = _spriteSheets.find(id);
    if (_spriteSheets.end() == itSheet)
        return nullptr;

    return itSheet->second;
}

std::optional<Animation2DSprite> Animation2DManager::FindSprite(
  Ptr<Animation2DSpriteSheet> spriteSheet, int32 spriteIndex)
{
    if (nullptr == spriteSheet)
        return std::nullopt;

    if (spriteIndex >= spriteSheet->GetSpriteCount() || spriteIndex < 0)
        return std::nullopt;

    return Animation2DSprite{spriteSheet, spriteIndex};
}

std::optional<Animation2DSprite> Animation2DManager::FindSprite(
  const std::string& spriteSheetName, int32 spriteIndex)
{
    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet(spriteSheetName);
    return FindSprite(sheet, spriteIndex);
}

Ptr<Animation2DClip> Animation2DManager::FindAnimationClip(const std::string& name)
{
    auto itFinder = _animationClipFinder.find(name);
    if (_animationClipFinder.end() == itFinder)
        return nullptr;

    int32 id     = itFinder->second;
    auto  itClip = _animationClips.find(id);
    if (_animationClips.end() == itClip)
        return nullptr;

    return itClip->second;
}

Ptr<Animation2DSequence> Animation2DManager::FindAnimationSequence(const std::string& name)
{
    auto itFinder = _animationSequenceFinder.find(name);
    if (_animationSequenceFinder.end() == itFinder)
        return nullptr;

    int32 id         = itFinder->second;
    auto  itSequence = _animationSequences.find(id);
    if (_animationSequences.end() == itSequence)
        return nullptr;

    return itSequence->second;
}

bool Animation2DManager::CreateSpriteSheet(const std::string& name, Ptr<Texture> texture)
{
    if (nullptr == texture)
        return false;

    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet(name);

    if (sheet)
        return false;

    sheet = New<Animation2DSpriteSheet>();
    if (!sheet->Init(texture))
    {
        DESTROY(sheet)
        return false;
    }

    sheet->SetName(name);
    sheet->SetID(_spriteSheetIDCounter);
    _spriteSheets[_spriteSheetIDCounter] = sheet;
    _spriteSheetFinder[name]             = _spriteSheetIDCounter;
    _spriteSheetIDCounter++;

    return true;
}

bool Animation2DManager::CreateSpriteSheet(
  const std::string& spriteSheetName, const std::string& textureName)
{
    Ptr<Texture> texture = FIND_TEXTURE(textureName);
    return CreateSpriteSheet(spriteSheetName, texture);
}

bool Animation2DManager::CreateSpriteSheet(
  const std::string& spriteSheetName, const std::string& textureName, const std::wstring& fileName)
{
    Ptr<Texture> texture = TEXTURE_MANAGER->LoadTexture(textureName, fileName);
    return CreateSpriteSheet(spriteSheetName, texture);
}

bool Animation2DManager::CreateAnimationClip(const std::string& name)
{
    Ptr<Animation2DClip> clip = FindAnimationClip(name);

    if (clip)
        return false;

    clip = New<Animation2DClip>();

    clip->SetName(name);
    clip->SetID(_animationClipIDCounter);
    _animationClips[_animationClipIDCounter] = clip;
    _animationClipFinder[name]               = _animationClipIDCounter;
    _animationClipIDCounter++;

    return true;
}

bool Animation2DManager::CreateAnimationSequence(const std::string& name)
{
    Ptr<Animation2DSequence> sequence = FindAnimationSequence(name);

    if (sequence)
        return false;

    sequence = New<Animation2DSequence>();

    sequence->SetName(name);
    sequence->SetID(_animationSequenceIDCounter);
    _animationSequences[_animationSequenceIDCounter] = sequence;
    _animationSequenceFinder[name]                   = _animationSequenceIDCounter;
    _animationSequenceIDCounter++;

    return true;
}

void Animation2DManager::AddSprite(const std::string& spriteSheetName, Vector2 start, Vector2 size)
{
    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet(spriteSheetName);
    if (nullptr == sheet)
        return;

    sheet->AddSpriteData(start, size);
}

void Animation2DManager::AddSprite(
  const std::string& spriteSheetName, float startX, float startY, float sizeX, float sizeY)
{
    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet(spriteSheetName);
    if (nullptr == sheet)
        return;

    sheet->AddSpriteData(startX, startY, sizeX, sizeY);
}

void Animation2DManager::AddFrame(
  const std::string& clipName, Ptr<Animation2DSpriteSheet> spriteSheet, int32 spriteIndex)
{
    Ptr<Animation2DClip> clip = FindAnimationClip(clipName);
    if (nullptr == clip)
        return;

    clip->AddFrameData(spriteSheet, spriteIndex);
}

void Animation2DManager::AddFrame(
  const std::string& clipName, const std::string& spriteSheetName, int32 spriteIndex)
{
    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet(spriteSheetName);
    AddFrame(clipName, sheet, spriteIndex);
}

void Animation2DManager::AddClip(const std::string& sequenceName, Ptr<Animation2DClip> clip)
{
    Ptr<Animation2DSequence> sequence = FindAnimationSequence(sequenceName);
    if (nullptr == sequence)
        return;

    sequence->AddAnimationClip(clip);
}

void Animation2DManager::AddClip(const std::string& sequenceName, const std::string& clipName)
{
    Ptr<Animation2DClip> clip = FindAnimationClip(clipName);
    AddClip(sequenceName, clip);
}
