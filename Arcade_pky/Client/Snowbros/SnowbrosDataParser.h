#pragma once

#include "Tilemap/Tile.h"

struct TileMetadata
{
    std::vector<byte> paletteNumbers;

    std::map<TileType::Type, std::set<uint32>> tiles;

    uint16 tileSize      = 1;
    uint16 tileTypeCount = 0;
};

class SnowbrosDataParser
{
public:
    static bool ParseStageData(const std::wstring& filename, byte* pData, size_t dataLength);
    static bool ParseTileMetadata(TileMetadata& data);
    static bool ParseIndexedTexture(const std::string& name, const std::wstring& filename);
    static bool ParseAnimationData(const std::string& textureName,
      const std::wstring&                             textureFilename,
      const std::string&                              spriteSheetName,
      const std::wstring&                             spriteDataFilename);
    static bool ReadPalettes(const std::wstring& filename);
};
