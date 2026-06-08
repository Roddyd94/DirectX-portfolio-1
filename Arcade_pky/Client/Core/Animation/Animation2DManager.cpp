#include "pch.h"

#include "Animation2DManager.h"

#include "Core/ResourceManager.h"

#include "Animation2DClip.h"
#include "Animation2DSequence.h"
#include "Animation2DSprite.h"

bool Animation2DManager::Init()
{
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
    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet<Animation2DSpriteSheet>(spriteSheetName);
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

Ptr<class Animation2DSpriteSheet> Animation2DManager::CreateSpriteSheet(
  const std::string& name, Ptr<class Texture> texture)
{
    if (nullptr == texture)
        return nullptr;

    Ptr<TextureAnimation2DSpriteSheet> sheet = FindSpriteSheet<TextureAnimation2DSpriteSheet>(name);

    if (nullptr != sheet && sheet->GetTexture() == texture)
        return sheet;

    sheet = New<TextureAnimation2DSpriteSheet>();
    if (!sheet->Init(texture))
    {
        DESTROY(sheet)
        return nullptr;
    }

    sheet->SetName(name);
    sheet->SetID(_spriteSheetIDCounter);
    _spriteSheets[_spriteSheetIDCounter] = sheet;
    _spriteSheetFinder[name]             = _spriteSheetIDCounter;
    _spriteSheetIDCounter++;

    return sheet;
}

Ptr<class Animation2DSpriteSheet> Animation2DManager::CreateSpriteSheet(
  const std::string& spriteSheetName, const std::string& textureName)
{
    Ptr<Texture> texture = FIND_TEXTURE(textureName);
    return CreateSpriteSheet(spriteSheetName, texture);
}

Ptr<class Animation2DSpriteSheet> Animation2DManager::CreateSpriteSheet(
  const std::string& spriteSheetName, const std::string& textureName, const std::wstring& fileName)
{
    Ptr<Texture> texture = TEXTURE_MANAGER->LoadTexture(textureName, fileName);
    return CreateSpriteSheet(spriteSheetName, texture);
}

Ptr<class Animation2DSpriteSheet> Animation2DManager::CreateIndexedSpriteSheet(
  const std::string& name, Ptr<class IndexedTexture> texture)
{
    if (nullptr == texture)
        return nullptr;

    Ptr<IndexedTextureAnimation2DSpriteSheet> sheet
      = FindSpriteSheet<IndexedTextureAnimation2DSpriteSheet>(name);

    if (nullptr != sheet && sheet->GetTexture() == texture)
        return sheet;

    sheet = New<IndexedTextureAnimation2DSpriteSheet>();
    if (!sheet->Init(texture))
    {
        DESTROY(sheet)
        return nullptr;
    }

    sheet->SetName(name);
    sheet->SetID(_spriteSheetIDCounter);
    _spriteSheets[_spriteSheetIDCounter] = sheet;
    _spriteSheetFinder[name]             = _spriteSheetIDCounter;
    _spriteSheetIDCounter++;

    return sheet;
}

Ptr<class Animation2DSpriteSheet> Animation2DManager::CreateIndexedSpriteSheet(
  const std::string& spriteSheetName, const std::string& textureName)
{
    Ptr<IndexedTexture> texture = FIND_INDEXED_TEXTURE(textureName);
    return CreateIndexedSpriteSheet(spriteSheetName, texture);
}

Ptr<class Animation2DClip> Animation2DManager::CreateAnimationClip(const std::string& name)
{
    Ptr<Animation2DClip> clip = FindAnimationClip(name);

    if (clip)
        return clip;

    clip = New<Animation2DClip>();

    clip->SetName(name);
    clip->SetID(_animationClipIDCounter);
    _animationClips[_animationClipIDCounter] = clip;
    _animationClipFinder[name]               = _animationClipIDCounter;
    _animationClipIDCounter++;

    return clip;
}

Ptr<class Animation2DSequence> Animation2DManager::CreateAnimationSequence(const std::string& name)
{
    Ptr<Animation2DSequence> sequence = FindAnimationSequence(name);

    if (sequence)
        return sequence;

    sequence = New<Animation2DSequence>();

    sequence->SetName(name);
    sequence->SetID(_animationSequenceIDCounter);
    _animationSequences[_animationSequenceIDCounter] = sequence;
    _animationSequenceFinder[name]                   = _animationSequenceIDCounter;
    _animationSequenceIDCounter++;

    return sequence;
}

void Animation2DManager::AddSprite(const std::string& spriteSheetName, Vector2 start, Vector2 size)
{
    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet<Animation2DSpriteSheet>(spriteSheetName);
    if (nullptr == sheet)
        return;

    sheet->AddSpriteData(start, size);
}

void Animation2DManager::AddSprite(
  const std::string& spriteSheetName, float startX, float startY, float sizeX, float sizeY)
{
    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet<Animation2DSpriteSheet>(spriteSheetName);
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
    Ptr<Animation2DSpriteSheet> sheet = FindSpriteSheet<Animation2DSpriteSheet>(spriteSheetName);
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
