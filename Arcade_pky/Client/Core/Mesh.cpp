#include "pch.h"

#include "Mesh.h"

#include "DeviceManager.h"
#include "MaterialManager.h"
#include "ResourceManager.h"

void Mesh::Destroy() {}

bool Mesh::CreateMesh(void* vertexData,
  uint32                    size,
  uint32                    count,
  D3D11_USAGE               vertexUsage,
  D3D11_PRIMITIVE_TOPOLOGY  primitive,
  void*                     indexData,
  uint32                    indexSize,
  uint32                    indexCount,
  DXGI_FORMAT               format,
  D3D11_USAGE               indexUsage)
{
    _vertexBufferData.size  = size;
    _vertexBufferData.count = count;
    _vertexBufferData.data.resize(size * count);
    memcpy(_vertexBufferData.data.data(), vertexData, size * count);

    if (!CreateBuffer(_vertexBufferData.buffer, D3D11_BIND_VERTEX_BUFFER,
          vertexData, size, count, vertexUsage))
        return false;

    _primitive = primitive;

    if (indexData)
    {
        MeshSlot slot;

        slot.indexBufferData.size  = indexSize;
        slot.indexBufferData.count = indexCount;
        slot.indexBufferData.data.resize(indexSize * indexCount);
        slot.indexBufferData.format = format;
        memcpy(
          slot.indexBufferData.data.data(), indexData, indexSize * indexCount);

        if (!CreateBuffer(slot.indexBufferData.buffer, D3D11_BIND_INDEX_BUFFER,
              indexData, indexSize, indexCount, indexUsage))
            return false;

        slot.material
          = MATERIAL_MANAGER->CreateMaterialInstance("DefaultMaterial");
        _meshSlots.push_back(slot);
    }

    return true;
}

void Mesh::Render()
{
    uint32 stride = _vertexBufferData.size;
    uint32 offset = 0;

    CONTEXT->IASetPrimitiveTopology(_primitive);
    CONTEXT->IASetVertexBuffers(
      0, 1, _vertexBufferData.buffer.GetAddressOf(), &stride, &offset);

    size_t size = _meshSlots.size();
    if (size > 0)
    {
        for (size_t i = 0; i < size; ++i)
        {
            CONTEXT->IASetIndexBuffer(
              _meshSlots[i].indexBufferData.buffer.Get(),
              _meshSlots[i].indexBufferData.format, 0);
            CONTEXT->DrawIndexed(_meshSlots[i].indexBufferData.count, 0, 0);
        }
    }
    else
    {
        CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
        CONTEXT->Draw(_vertexBufferData.count, 0);
    }
}

void Mesh::RenderInstancing(uint32 count)
{
    uint32 stride = _vertexBufferData.size;
    uint32 offset = 0;

    CONTEXT->IASetPrimitiveTopology(_primitive);
    CONTEXT->IASetVertexBuffers(
      0, 1, _vertexBufferData.buffer.GetAddressOf(), &stride, &offset);
    CONTEXT->IASetIndexBuffer(_meshSlots[0].indexBufferData.buffer.Get(),
      _meshSlots[0].indexBufferData.format, 0);

    CONTEXT->DrawIndexedInstanced(
      _meshSlots[0].indexBufferData.count, count, 0, 0, 0);
}

const MeshSlot* Mesh::GetSlot(uint32 index)
{
    if (_meshSlots.size() <= index)
        return nullptr;

    return &_meshSlots[index];
}

bool Mesh::CreateBuffer(ComPtr<ID3D11Buffer>& buffer,
  D3D11_BIND_FLAG                             flag,
  void*                                       data,
  uint32                                      size,
  uint32                                      count,
  D3D11_USAGE                                 usage)
{
    D3D11_BUFFER_DESC bufferDesc = {};

    bufferDesc.ByteWidth           = size * count;
    bufferDesc.Usage               = usage;
    bufferDesc.BindFlags           = flag;
    bufferDesc.StructureByteStride = size;

    if (usage == D3D11_USAGE_DYNAMIC)
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    else if (usage == D3D11_USAGE_STAGING)
        bufferDesc.CPUAccessFlags
          = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

    D3D11_SUBRESOURCE_DATA bufferData = {};
    bufferData.pSysMem                = data;

    if (FAILED(DEVICE->CreateBuffer(
          &bufferDesc, &bufferData, buffer.GetAddressOf())))
        return false;

    return true;
}
