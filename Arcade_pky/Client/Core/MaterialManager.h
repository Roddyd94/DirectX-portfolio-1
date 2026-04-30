#pragma once
#include "SubManager.h"

#include "Rendering.h"

class MaterialManager : public SubManager
{
public:
    MaterialManager()           = default;
    ~MaterialManager() override = default;
    DELETE_SPECIAL_FUNC(MaterialManager)

private:
    std::unordered_map<std::string, Ptr<class Material>> _materials;

public:
    bool Init() override;
    void Destroy() override;

    Ptr<class Material> FindMaterial(const std::string& name);

    Ptr<class Material> CreateMaterial(const std::string& name,
      const std::string&                                  pixelShaderName,
      uint8                                               samplerType);
    Ptr<class Material> CreateMaterialInstance(const std::string& name);
};
