#pragma once
#include "SubManager.h"
class MeshManager : public SubManager
{
public:
    MeshManager()           = default;
    ~MeshManager() override = default;
    DELETE_SPECIAL_FUNC(MeshManager)
private:
    std::unordered_map<std::string, Ptr<class Mesh>> _meshes;

public:
    bool Init() override;
    void Destroy() override;

    Ptr<class Mesh> FindMesh(const std::string& name);
    Ptr<class Mesh> CreateMesh(const std::string& name,
      void*                                       vertexData,
      uint32                                      size,
      int32                                       count,
      D3D11_USAGE                                 vertexUsage,
      D3D11_PRIMITIVE_TOPOLOGY                    primitive,
      void*                                       indexData  = nullptr,
      uint32                                      indexSize  = 0,
      int32                                       indexCount = 0,
      DXGI_FORMAT                                 fmt = DXGI_FORMAT_UNKNOWN,
      D3D11_USAGE indexUsage                          = D3D11_USAGE_DEFAULT);
};
