#include "pch.h"

#include "SnowbrosDataParser.h"

#include "Core/Animation/Animation2DManager.h"
#include "Core/DirectoryManager.h"
#include "Core/ResourceManager.h"
#include "Core/TextureManager.h"

#include "Core/Animation/Animation2DClip.h"
#include "Core/Palette.h"
#include "Core/PaletteStructureBuffer.h"
#include "Core/Texture.h"

#include <fstream>

bool SnowbrosDataParser::ParseStageData(const std::wstring& filename, std::vector<StageData>& data)
{
    auto path = DirectoryManager::Instance().GetCachePath("Resources/Stage");

    if (path == std::nullopt)
        return false;

    auto filePath = path.value();
    DirectoryManager::Instance().GetFile(filePath, filename, filePath);

    std::fstream fs{filePath.native(), fs.binary | fs.in};

    int16 stageCount;

    fs.read(reinterpret_cast<char*>(&stageCount), sizeof(int16));
    data.resize(stageCount);

    for (int i = 0; i < stageCount; ++i)
    {
        data[i].number = i;
        int16 stageFilenameLength;
        fs.read(reinterpret_cast<char*>(&stageFilenameLength), sizeof(int16));

        data[i].filename.resize(stageFilenameLength);
        fs.read(
          reinterpret_cast<char*>(data[i].filename.data()), stageFilenameLength * sizeof(wchar_t));

        fs.read(reinterpret_cast<char*>(&data[i].paletteNumber), sizeof(int16));

        for (int j = 0; j < 2; ++j)
        {
            fs.read(reinterpret_cast<char*>(&data[i].playerPosition[j]), sizeof(Vector2));
            fs.read(reinterpret_cast<char*>(&data[i].playerDirection[j]), sizeof(float));
        }

        int16 enemiesCount;
        fs.read(reinterpret_cast<char*>(&enemiesCount), sizeof(int16));
        data[i].enemies.resize(enemiesCount);

        for (int j = 0; j < enemiesCount; ++j)
        {
            int16 enemyType;
            fs.read(reinterpret_cast<char*>(&enemyType), sizeof(int16));
            EnemyData& enemyData = data[i].enemies[j];
            enemyData.type       = static_cast<SnowbrosEnemyType>(enemyType);

            fs.read(reinterpret_cast<char*>(&enemyData.position.x), sizeof(float));
            fs.read(reinterpret_cast<char*>(&enemyData.position.y), sizeof(float));
            fs.read(reinterpret_cast<char*>(&enemyData.direction), sizeof(float));

            int16 patrolPositionCount;
            fs.read(reinterpret_cast<char*>(&patrolPositionCount), sizeof(int16));
            EnemyPatrolData& patrolData = enemyData.patrolData;
            patrolData.patrolPoints.resize(patrolPositionCount);

            for (int k = 0; k < patrolPositionCount; ++k)
            {
                Vector2 patrolPosition;
                fs.read(reinterpret_cast<char*>(&patrolData.patrolPoints[k]), sizeof(Vector2));
            }

            int16 loopPatrol;
            fs.read(reinterpret_cast<char*>(&patrolData.loopCount), sizeof(int16));
        }
    }

    return true;
}

bool SnowbrosDataParser::ParseStageMapData(
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

bool SnowbrosDataParser::ParseTileMetadata(TileMetadata& data)
{
    auto path = DirectoryManager::Instance().GetCachePath("Resources/Stage");

    if (path == std::nullopt)
        return false;

    auto filePath = path.value();
    DirectoryManager::Instance().GetFile(filePath, "snowbros_tilemap_metadata.bin", filePath);

    std::fstream fs{filePath.native(), fs.binary | fs.in};
    fs.read(reinterpret_cast<char*>(&data.tileSize), sizeof(data.tileSize));

    int16 tileLength;
    fs.read(reinterpret_cast<char*>(&tileLength), sizeof(tileLength));
    data.paletteNumbers.resize(tileLength);

    for (int i = 0; i < tileLength; ++i)
        fs.read(reinterpret_cast<char*>(&data.paletteNumbers[i]), sizeof(byte));

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

bool SnowbrosDataParser::ParseIndexedTexture(const std::string& name, const std::wstring& filename)
{
    auto texturePath = DirectoryManager::Instance().GetCachePath("Resources/Texture");

    if (texturePath == std::nullopt)
        return false;

    auto textureFilePath = texturePath.value();
    DirectoryManager::Instance().GetFile(textureFilePath, filename, textureFilePath);

    std::fstream fs{textureFilePath.native(), fs.binary | fs.in};

    std::vector<char> buf;
    buf.resize(14);
    fs.read(buf.data(), 14);

    uint32 headerSizeV5;
    fs.read(reinterpret_cast<char*>(&headerSizeV5), sizeof(uint32));

    headerSizeV5 -= sizeof(uint32);
    buf.resize(headerSizeV5);
    fs.read(buf.data(), headerSizeV5);

    int32 textureWidth;
    int32 textureHeight;
    int16 bitsPerPixel;
    int32 imageSize;
    int32 colorsInTable;
    memcpy_s(&textureWidth, sizeof(int32), reinterpret_cast<char*>(&buf[0]), sizeof(int32));
    memcpy_s(&textureHeight, sizeof(int32), reinterpret_cast<char*>(&buf[4]), sizeof(int32));
    memcpy_s(&bitsPerPixel, sizeof(int16), reinterpret_cast<char*>(&buf[10]), sizeof(int16));
    memcpy_s(&imageSize, sizeof(int32), reinterpret_cast<char*>(&buf[16]), sizeof(int32));
    memcpy_s(&colorsInTable, sizeof(int32), reinterpret_cast<char*>(&buf[28]), sizeof(int32));

    if (0 == colorsInTable)
        colorsInTable = 1 << bitsPerPixel;

    buf.resize(colorsInTable * sizeof(int32));
    fs.read(buf.data(), colorsInTable * sizeof(int32));

    buf.resize(imageSize);
    int32 byteWidth = imageSize / textureHeight;
    for (int32 i = 0; i < textureHeight; ++i)
        fs.read(&buf[byteWidth * (textureHeight - 1 - i)], byteWidth);

    Ptr<IndexedTexture> texture = New<IndexedTexture>();
    texture->SetName(name);
    texture->LoadTexture(buf.data(), textureWidth, textureHeight, bitsPerPixel);

    TEXTURE_MANAGER->AddIndexedTexture(name, texture);

    return true;
}

bool SnowbrosDataParser::ParseAnimationData(const std::string& textureName,
  const std::wstring&                                          textureFilename,
  const std::string&                                           spriteSheetName,
  const std::wstring&                                          animationDataFilename)
{
    auto dataPath = DirectoryManager::Instance().GetCachePath("Resources/Stage");

    if (dataPath == std::nullopt)
        return false;

    TEXTURE_MANAGER->LoadTexture(textureName, textureFilename);
    //ParseIndexedTexture(textureName, textureFilename);

    ANIMATION_MANAGER->CreateSpriteSheet(spriteSheetName, textureName);

    auto filePath = dataPath.value();
    DirectoryManager::Instance().GetFile(filePath, animationDataFilename, filePath);
    std::fstream fs{filePath.native(), fs.binary | fs.in};

    uint16 spriteCount;
    fs.read(reinterpret_cast<char*>(&spriteCount), sizeof(uint16));
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

    uint16 sequenceCount;
    fs.read(reinterpret_cast<char*>(&sequenceCount), sizeof(uint16));
    for (size_t i = 0; i < sequenceCount; i++)
    {
        uint16 sequenceNameLength;
        fs.read(reinterpret_cast<char*>(&sequenceNameLength), sizeof(uint16));

        std::string sequenceName;
        sequenceName.resize(sequenceNameLength);
        fs.read(sequenceName.data(), sequenceNameLength);
        ANIMATION_MANAGER->CreateAnimationSequence(sequenceName);
    }

    uint16 clipCount;
    fs.read(reinterpret_cast<char*>(&clipCount), sizeof(uint16));
    for (size_t i = 0; i < clipCount; i++)
    {
        uint16 clipNameLength;
        fs.read(reinterpret_cast<char*>(&clipNameLength), sizeof(uint16));

        std::string clipName;
        clipName.resize(clipNameLength);
        fs.read(clipName.data(), clipNameLength);
        Ptr<Animation2DClip> clip = ANIMATION_MANAGER->CreateAnimationClip(clipName);

        uint16 sequenceNameLength;
        fs.read(reinterpret_cast<char*>(&sequenceNameLength), sizeof(uint16));

        std::string sequenceName;
        sequenceName.resize(sequenceNameLength);
        fs.read(sequenceName.data(), sequenceNameLength);
        ANIMATION_MANAGER->AddClip(sequenceName, clipName);

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

bool SnowbrosDataParser::ReadPalettes(const std::wstring& filename)
{
    auto path = DirectoryManager::Instance().GetCachePath("Resources/Stage");

    if (path == std::nullopt)
        return false;

    auto filePath = path.value();
    DirectoryManager::Instance().GetFile(filePath, filename, filePath);
    std::fstream fs{filePath.native(), fs.binary | fs.in};

    uint16 paletteCount;
    fs.read(reinterpret_cast<char*>(&paletteCount), sizeof(uint16));

    auto paletteBuffer = FIND_STRUCTURE_BUFFER("Palette", PaletteStructureBuffer);
    paletteBuffer->SetPaletteLength(16);
    for (int i = 0; i < paletteCount; ++i)
    {
        uint16 colorNameLength;
        fs.read(reinterpret_cast<char*>(&colorNameLength), sizeof(uint16));

        std::string colorName;
        colorName.resize(colorNameLength);
        fs.read(colorName.data(), colorNameLength);

        auto palette = PALETTE_MANAGER->AddPalette(colorName);

        uint16 colorCount;
        fs.read(reinterpret_cast<char*>(&colorCount), sizeof(uint16));

        for (int j = 0; j < colorCount; ++j)
        {
            Color color;
            fs.read(reinterpret_cast<char*>(&color), sizeof(Color));
            palette->AddColor(color);
        }

        paletteBuffer->AddData(palette);
    }

    return true;
}
