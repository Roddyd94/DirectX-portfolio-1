#pragma once

#include "Types.h"
#include "Tilemap/Tile.h"

class SnowbrosDataParser
{
public:
    static bool ParseStageData(
      const std::wstring& filename, std::vector<StageData>& data);
    static bool ParseStageMapData(const std::wstring& filename, byte* pData, size_t dataLength);
    static bool ParseTileMetadata(TileMetadata& data);
    static bool ParseIndexedTexture(const std::string& name, const std::wstring& filename);
    static bool ParseAnimationData(const std::string& textureName,
      const std::wstring&                             textureFilename,
      const std::string&                              spriteSheetName,
      const std::wstring&                             spriteDataFilename);
    static bool ReadPalettes(const std::wstring& filename);
};
