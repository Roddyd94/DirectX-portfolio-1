#pragma once
#include "MaterialManager.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include "Resource.h"
#include "Singleton.h"

class ResourceManager : public Singleton<ResourceManager>
{
    DECLARE_SINGLETON(ResourceManager)
private:
    std::map<ResourceType, Ptr<class SubManager>> _managers;

public:
    bool Init();
    void Destroy();

    template <typename T>
    Ptr<T> Get(ResourceType type)
    {
        if (_managers.end() == _managers.find(type))
            return nullptr;

        return Cast<SubManager, T>(_managers[type]);
    }

    template <typename T>
    bool RegisterManager(ResourceType type)
    {
        Ptr<T> manager = New<T>();
        if (!manager->Init())
        {
            DESTROY(manager);
            return false;
        }

        _managers[type] = manager;

        return true;
    }
};

#define SHADER_MANAGER                                                         \
    ResourceManager::Instance().Get<ShaderManager>(ResourceType::Shader)
#define TEXTURE_MANAGER                                                        \
    ResourceManager::Instance().Get<TextureManager>(ResourceType::Texture)
#define MATERIAL_MANAGER                                                       \
    ResourceManager::Instance().Get<MaterialManager>(ResourceType::Material)
#define MESH_MANAGER                                                           \
    ResourceManager::Instance().Get<MeshManager>(ResourceType::Mesh)
#define ANIMATION_MANAGER                                                      \
    ResourceManager::Instance().Get<AnimationManager>(ResourceType::Animation)
#define SOUND_MANAGER                                                          \
    ResourceManager::Instance().Get<SoundManager>(ResourceType::Sound)
#define FONT_MANAGER                                                           \
    ResourceManager::Instance().Get<FontManager>(ResourceType::Font)

#define FIND_SHADER(x, T)          SHADER_MANAGER->FindShader<T>(x)
#define FIND_CONSTANT_BUFFER(x, T) SHADER_MANAGER->FindConstantBuffer<T>(x)
// #define FIND_STRUCTURE_BUFFER(x, T) SHADER_MANAGER->FindStructureBuffer<T>(x)

#define MESH_LINE_RECT    MESH_MANAGER->FindMesh("FrameRect")
#define MESH_LINE_SPHERE  MESH_MANAGER->FindMesh("FrameSphere")
#define MESH_TEXTURE_RECT MESH_MANAGER->FindMesh("TextureRect")
