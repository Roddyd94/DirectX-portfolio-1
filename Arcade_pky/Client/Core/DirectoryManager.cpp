#include "pch.h"

#include "DirectoryManager.h"

void DirectoryManager::Init()
{
    _rootPath = std::filesystem::current_path();
    if (!std::filesystem::exists(_rootPath))
        return;

    if (!std::filesystem::is_directory(_rootPath))
        return;

    _rootParentPath = _rootPath.parent_path();
    if (!std::filesystem::exists(_rootParentPath))
        return;

    if (!std::filesystem::is_directory(_rootParentPath))
        return;

    RegisterPath("Logs");
    RegisterPath("HLSL");
    RegisterPath("Resources");
}

void DirectoryManager::Destroy()
{
    _paths.clear();
}

std::optional<std::filesystem::path> DirectoryManager::GetCachePath(
  const std::string& pathName) const
{
    if (auto it = _paths.find(pathName); it != _paths.end())
        return it->second;

    return std::nullopt;
}

bool DirectoryManager::GetDirectoryFromRoot(
  const std::string& directoryName, OUT std::filesystem::path& outPath)
{
    std::filesystem::path dir = directoryName;
    GetDirectoryFromRoot(dir, outPath);

    return true;
}

bool DirectoryManager::GetDirectoryFromRoot(
  const std::filesystem::path& relativePath, OUT std::filesystem::path& outPath)
{
    std::filesystem::path foundPath = _rootPath / relativePath;

    if (!std::filesystem::exists(foundPath))
        return false;

    if (!std::filesystem::is_directory(foundPath))
        return false;

    outPath = foundPath;

    return true;
}

bool DirectoryManager::GetDirectory(const std::filesystem::path& basePath,
  const std::filesystem::path&                                   dirName,
  OUT std::filesystem::path& outPath)
{
    std::filesystem::path foundPath = basePath / dirName;

    if (!std::filesystem::exists(foundPath))
        return false;

    if (!std::filesystem::is_directory(foundPath))
        return false;

    outPath = foundPath;

    return true;
}

bool DirectoryManager::GetFile(const std::filesystem::path& basePath,
  const std::filesystem::path&                              fileName,
  OUT std::filesystem::path& outPath)
{
    std::filesystem::path foundPath = basePath / fileName;

    if (!std::filesystem::exists(foundPath))
        return false;

    if (!std::filesystem::is_regular_file(foundPath))
        return false;

    outPath = foundPath;

    return true;
}

bool DirectoryManager::IsFile(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
        return false;

    if (!std::filesystem::is_regular_file(path))
        return false;

    return true;
}

bool DirectoryManager::IsDirectory(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
        return false;

    if (!std::filesystem::is_directory(path))
        return false;

    return true;
}

bool DirectoryManager::CheckExtension(
  const std::filesystem::path& path, const std::string& extension)
{
    std::filesystem::path fileExtension = path.extension();
    std::string           strExtension  = fileExtension.string();

    std::string compareExtension = extension;
    std::transform(
      strExtension.begin(), strExtension.end(), strExtension.begin(), toupper);
    std::transform(compareExtension.begin(), compareExtension.end(),
      compareExtension.begin(), toupper);

    if (compareExtension == strExtension)
        return true;

    return false;
}

void DirectoryManager::RegisterPath(const std::string& path)
{
    std::filesystem::path registeredPath = _rootParentPath / path;
    if (!std::filesystem::exists(registeredPath))
        std::filesystem::create_directories(registeredPath);

    if (!std::filesystem::is_directory(registeredPath))
        return;

    _paths[path] = registeredPath;
}
