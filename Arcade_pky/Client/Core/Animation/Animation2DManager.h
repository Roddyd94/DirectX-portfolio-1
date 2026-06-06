#pragma once
#include "Core/SubManager.h"

#include "Animation2DSprite.h"

class Animation2DManager : public SubManager
{
public:
    Animation2DManager()           = default;
    ~Animation2DManager() override = default;
    DELETE_SPECIAL_FUNC(Animation2DManager)

private:
    std::unordered_map<std::string, int32> _spriteSheetFinder;
    std::unordered_map<std::string, int32> _animationClipFinder;
    std::unordered_map<std::string, int32> _animationSequenceFinder;

    std::map<int32, Ptr<class Animation2DSpriteSheet>> _spriteSheets;
    std::map<int32, Ptr<class Animation2DClip>>        _animationClips;
    std::map<int32, Ptr<class Animation2DSequence>>    _animationSequences;

    int32 _spriteSheetIDCounter       = 0;
    int32 _animationClipIDCounter     = 0;
    int32 _animationSequenceIDCounter = 0;

public:
    bool Init();
    void Destroy() override;

    std::optional<Animation2DSprite> FindSprite(
      Ptr<class Animation2DSpriteSheet> spriteSheet, int32 spriteIndex);
    std::optional<Animation2DSprite> FindSprite(
      const std::string& spriteSheetName, int32 spriteIndex);

    Ptr<class Animation2DClip>     FindAnimationClip(const std::string& name);
    Ptr<class Animation2DSequence> FindAnimationSequence(const std::string& name);

    Ptr<class Animation2DSpriteSheet> CreateSpriteSheet(
      const std::string& name, Ptr<class Texture> texture);
    Ptr<class Animation2DSpriteSheet> CreateSpriteSheet(
      const std::string& spriteSheetName, const std::string& textureName);
    Ptr<class Animation2DSpriteSheet> CreateSpriteSheet(const std::string& spriteSheetName,
      const std::string&                                                   textureName,
      const std::wstring&                                                  fileName);
    
    Ptr<class Animation2DSpriteSheet> CreateIndexedSpriteSheet(
      const std::string& name, Ptr<class IndexedTexture> texture);

    Ptr<class Animation2DClip>     CreateAnimationClip(const std::string& name);
    Ptr<class Animation2DSequence> CreateAnimationSequence(const std::string& name);

    void AddSprite(const std::string& spriteSheetName, Vector2 start, Vector2 size);
    void AddSprite(
      const std::string& spriteSheetName, float startX, float startY, float sizeX, float sizeY);
    void AddFrame(const std::string&    clipName,
      Ptr<class Animation2DSpriteSheet> spriteSheet,
      int32                             spriteIndex);
    void AddFrame(
      const std::string& clipName, const std::string& spriteSheetName, int32 spriteIndex);
    void AddClip(const std::string& sequenceName, Ptr<class Animation2DClip> clip);
    void AddClip(const std::string& sequenceName, const std::string& clipName);

public:
    template <typename T>
    Ptr<T> FindSpriteSheet(const std::string& name)
    {
        auto itFinder = _spriteSheetFinder.find(name);
        if (_spriteSheetFinder.end() == itFinder)
            return nullptr;

        int32 id      = itFinder->second;
        auto  itSheet = _spriteSheets.find(id);
        if (_spriteSheets.end() == itSheet)
            return nullptr;

        return Cast<Animation2DSpriteSheet, T>(itSheet->second);
    }
};
