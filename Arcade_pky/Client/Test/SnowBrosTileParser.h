#pragma once

#include "Core/DirectoryManager.h"

#include <fstream>

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

        filename = filePath.native();
        std::fstream fs{ filename, fs.binary | fs.in };

        fs.read(reinterpret_cast<char*>(pData), dataLength);
    }
};
