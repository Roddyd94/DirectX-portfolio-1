#include "pch.h"

#include "ResourceManager.h"

#include "MaterialManager.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
#include "SubManager.h"
#include "TextureManager.h"

bool ResourceManager::Init()
{
    if (!RegisterManager<ShaderManager>(ResourceType::Shader))
        return false;

    if (!RegisterManager<TextureManager>(ResourceType::Texture))
        return false;

    if (!RegisterManager<MaterialManager>(ResourceType::Material))
        return false;

    if (!RegisterManager<MeshManager>(ResourceType::Mesh))
        return false;

    if (!RegisterManager<Animation2DManager>(ResourceType::Animation))
        return false;

    if (!RegisterManager<SoundManager>(ResourceType::Sound))
        return false;

    if (!RegisterManager<PaletteManager>(ResourceType::Palette))
        return false;

    // if (!RegisterManager<FontManager>(ResourceType::Font))
    //     return false;

    return true;
}

void ResourceManager::Destroy()
{
    for (auto& [_, manager] : _managers)
        DESTROY(manager)

    _managers.clear();
}
