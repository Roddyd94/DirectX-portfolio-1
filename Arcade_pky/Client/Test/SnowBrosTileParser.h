#pragma once

#include "Core/DirectoryManager.h"

#include "Tilemap/Tile.h"

#include <fstream>

struct TileMetadata
{
    std::map<TileType::Type, std::set<uint32>> tiles;

    uint16 tileSize      = 1;
    uint16 tileTypeCount = 0;
};

class SnowBrosTileParser
{
public:
    static bool ParseStageData(std::wstring filename, byte* pData, size_t dataLength)
    {
        auto path = DirectoryManager::Instance().GetCachePath("Resources/Stage");

        if (path == std::nullopt)
            return false;

        auto filePath = path.value();
        DirectoryManager::Instance().GetFile(filePath, filename, filePath);

        std::fstream fs{filePath.native(), fs.binary | fs.in};

        fs.read(reinterpret_cast<char*>(pData), dataLength);
    }

    static bool ParseTileMetadata(TileMetadata& data)
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
};
