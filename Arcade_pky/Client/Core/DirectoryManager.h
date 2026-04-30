#pragma once
#include "Singleton.h"

#include <filesystem>
#include <fstream>

class DirectoryManager : public Singleton<DirectoryManager>
{
    DECLARE_SINGLETON(DirectoryManager)

private:
    std::unordered_map<std::string, std::filesystem::path> _paths;

    std::filesystem::path _rootPath;
    std::filesystem::path _rootParentPath;

public:
    void Init();
    void Destroy();

    const std::filesystem::path& GetRootPath() const { return _rootPath; }

    std::optional<std::filesystem::path> GetCachePath(
      const std::string& pathName) const;

    bool GetDirectoryFromRoot(
      const std::string& directoryName, OUT std::filesystem::path& outPath);

    bool GetDirectoryFromRoot(const std::filesystem::path& relativePath,
      OUT std::filesystem::path& outPath);

    bool GetDirectory(const std::filesystem::path& basePath,
      const std::filesystem::path&                 dirName,
      OUT std::filesystem::path& outPath);

    bool GetFile(const std::filesystem::path& basePath,
      const std::filesystem::path&            fileName,
      OUT std::filesystem::path& outPath);

    bool IsFile(const std::filesystem::path& path);
    bool IsDirectory(const std::filesystem::path& path);
    bool CheckExtension(
      const std::filesystem::path& path, const std::string& extension);

private:
    void RegisterPath(const std::string& path);
};
