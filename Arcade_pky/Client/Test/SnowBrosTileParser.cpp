#include "pch.h"

#include "SnowBrosTileParser.h"

#include "Core/Animation/Animation2DManager.h"
#include "Core/DirectoryManager.h"
#include "Core/ResourceManager.h"
#include "Core/TextureManager.h"

#include "Core/Animation/Animation2DClip.h"
#include "Core/Texture.h"

#include <fstream>

namespace snowbros
{
    bool SnowBrosTileParser::ParseStageData(
      const std::wstring& filename, byte* pData, size_t dataLength)
    {
        auto path = DirectoryManager::Instance().GetCachePath("Resources/Stage");

        if (path == std::nullopt)
            return false;

        auto filePath = path.value();
        DirectoryManager::Instance().GetFile(filePath, filename, filePath);

        std::fstream fs{filePath.native(), fs.binary | fs.in};

        fs.read(reinterpret_cast<char*>(pData), dataLength);
    }

    bool SnowBrosTileParser::ParseTileMetadata(TileMetadata& data)
    {
        auto path = DirectoryManager::Instance().GetCachePath("Resources/Stage");

        if (path == std::nullopt)
            return false;

        auto filePath = path.value();
        DirectoryManager::Instance().GetFile(filePath, "snowbros_tilemap_metadata.bin", filePath);

        std::fstream fs{filePath.native(), fs.binary | fs.in};

        fs.read(reinterpret_cast<char*>(&data.tileSize), sizeof(data.tileSize));
        fs.read(reinterpret_cast<char*>(&data.tileTypeCount), sizeof(data.tileTypeCount));

        for (size_t i = 0; i < data.tileTypeCount; i++)
        {
            TileType::Type tileType;
            uint16         tileCount;
            fs.read(reinterpret_cast<char*>(&tileType), sizeof(tileType));
            fs.read(reinterpret_cast<char*>(&tileCount), sizeof(uint16));

            switch (data.tileSize)
            {
            case 1:
            {
                std::vector<uint8> tileData(tileCount);
                fs.read(reinterpret_cast<char*>(tileData.data()), sizeof(uint8) * tileCount);
                for (auto& tile : tileData)
                    data.tiles[tileType].insert(tile);
            }
            break;
            case 2:
            {
                std::vector<uint16> tileData(tileCount);
                fs.read(reinterpret_cast<char*>(tileData.data()), sizeof(uint16) * tileCount);
                for (auto& tile : tileData)
                    data.tiles[tileType].insert(tile);
            }
            break;
            case 4:
            {
                std::vector<uint32> tileData(tileCount);
                fs.read(reinterpret_cast<char*>(tileData.data()), sizeof(uint32) * tileCount);
                for (auto& tile : tileData)
                    data.tiles[tileType].insert(tile);
            }
            break;
            }
        }
    }

    bool SnowBrosTileParser::ParseAnimationData(const std::string& textureName,
      const std::wstring&                                          textureFilename,
      const std::string&                                           spriteSheetName,
      const std::wstring&                                          animationDataFilename)
    {
        auto path = DirectoryManager::Instance().GetCachePath("Resources/Stage");

        if (path == std::nullopt)
            return false;

        Ptr<Texture> texture = TEXTURE_MANAGER->LoadTexture(textureName, textureFilename);

        auto filePath = path.value();
        DirectoryManager::Instance().GetFile(filePath, animationDataFilename, filePath);

        std::fstream fs{filePath.native(), fs.binary | fs.in};

        uint16 spriteCount;
        fs.read(reinterpret_cast<char*>(&spriteCount), sizeof(uint16));
        ANIMATION_MANAGER->CreateSpriteSheet(spriteSheetName, texture);

        for (size_t i = 0; i < spriteCount; i++)
        {
            uint16 startX;
            uint16 startY;
            uint16 sizeX;
            uint16 sizeY;
            fs.read(reinterpret_cast<char*>(&startX), sizeof(uint16));
            fs.read(reinterpret_cast<char*>(&startY), sizeof(uint16));
            fs.read(reinterpret_cast<char*>(&sizeX), sizeof(uint16));
            fs.read(reinterpret_cast<char*>(&sizeY), sizeof(uint16));

            Vector2 start;
            Vector2 size;

            start.x = static_cast<float>(startX);
            start.y = static_cast<float>(startY);
            size.x  = static_cast<float>(sizeX);
            size.y  = static_cast<float>(sizeY);

            ANIMATION_MANAGER->AddSprite(spriteSheetName, start, size);
        }

        ANIMATION_MANAGER->CreateAnimationSequence(spriteSheetName);
        uint16 clipCount;
        fs.read(reinterpret_cast<char*>(&clipCount), sizeof(uint16));
        for (size_t i = 0; i < clipCount; i++)
        {
            uint16 clipNameLength;
            fs.read(reinterpret_cast<char*>(&clipNameLength), sizeof(uint16));

            char clipName[256] = {};
            fs.read(reinterpret_cast<char*>(&clipName), clipNameLength);

            Ptr<Animation2DClip> clip = ANIMATION_MANAGER->CreateAnimationClip(clipName);
            ANIMATION_MANAGER->AddClip(spriteSheetName, clipName);

            uint16 isLoop;
            fs.read(reinterpret_cast<char*>(&isLoop), sizeof(uint16));
            clip->SetLoop(isLoop);

            float frameTime;
            fs.read(reinterpret_cast<char*>(&frameTime), sizeof(float));
            clip->SetFrameTime(frameTime);

            uint16 frameCount;
            fs.read(reinterpret_cast<char*>(&frameCount), sizeof(uint16));

            for (size_t j = 0; j < frameCount; j++)
            {
                uint8 frameIndex;
                fs.read(reinterpret_cast<char*>(&frameIndex), sizeof(uint8));

                ANIMATION_MANAGER->AddFrame(clipName, spriteSheetName, frameIndex);
            }
        }
    }
} // namespace snowbros
