#pragma once
#include "Buffer.h"
#include "Material.h"
#include "Resource.h"

struct MeshSlot
{
    IndexBufferData     indexBufferData;
    Ptr<class Material> material;
};

class Mesh : public Resource
{
public:
    Mesh()          = default;
    virtual ~Mesh() = default;

protected:
    VertexBufferData         _vertexBufferData;
    std::vector<MeshSlot>    _meshSlots;
    D3D11_PRIMITIVE_TOPOLOGY _primitive;

public:
    void Destroy() override;

    bool CreateMesh(void*      vertexData,
      uint32                   size,
      int32                    count,
      D3D11_USAGE              vertexUsage,
      D3D11_PRIMITIVE_TOPOLOGY primitive,
      void*                    indexData,
      uint32                   indexSize,
      int32                    indexCount,
      DXGI_FORMAT              format,
      D3D11_USAGE              indexUsage = D3D11_USAGE_DEFAULT);

    void Render();
    void RenderInstancing(int32 count);

    const MeshSlot* GetSlot(uint32 index);
    size_t          GetSlotCount() { return _meshSlots.size(); }

private:
    bool CreateBuffer(ComPtr<ID3D11Buffer>& buffer,
      D3D11_BIND_FLAG                       flag,
      void*                                 data,
      uint32                                size,
      int32                                 count,
      D3D11_USAGE                           usage);
};
