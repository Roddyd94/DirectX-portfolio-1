#include "pch.h"

#include "MaterialManager.h"

#include "ResourceManager.h"
#include "ShaderManager.h"

#include "Material.h"
#include "MaterialConstantBuffer.h"

bool MaterialManager::Init()
{
    if (!CreateMaterial("DefaultMaterial", "StaticMeshShader", SamplerType::Point))
        return false;

    return true;
}

void MaterialManager::Destroy()
{
    for (auto& [_, material] : _materials)
        DESTROY(material)
}

Ptr<Material> MaterialManager::FindMaterial(const std::string& name)
{
    auto itFinder = _materialFinder.find(name);
    if (_materialFinder.end() == itFinder)
        return nullptr;

    int32 id         = itFinder->second;
    auto  itMaterial = _materials.find(id);
    if (_materials.end() == itMaterial)
        return nullptr;

    return itMaterial->second;
}

Ptr<Material> MaterialManager::CreateMaterial(
  const std::string& name, const std::string& pixelShaderName, uint8 samplerType)
{
    Ptr<Material> matFound = FindMaterial(name);
    if (matFound)
        return matFound;

    Ptr<Material> mat = New<Material>();
    mat->SetName(name);
    mat->SetID(_idCounter);
    mat->SetSamplerType(samplerType);
    mat->SetPixelShader(pixelShaderName);

    mat->_constantBuffer   = SHADER_MANAGER->FindConstantBuffer<MaterialConstantBuffer>("Material");
    _materials[_idCounter] = mat;
    _materialFinder[name]  = _idCounter;
    _idCounter++;

    return mat;
}

Ptr<Material> MaterialManager::CreateMaterialInstance(const std::string& name)
{
    Ptr<Material> origin = FindMaterial(name);
    if (nullptr == origin)
        return nullptr;

    return origin->Clone();
}
