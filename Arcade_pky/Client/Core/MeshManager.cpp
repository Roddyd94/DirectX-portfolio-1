#include "pch.h"

#include "MeshManager.h"

#include "Mesh.h"
#include "Rendering.h"

bool MeshManager::Init()
{
    Vector3 frameRectVertices[4]{
      Vector3(-0.5f, 0.5f, 0.f),
      Vector3(0.5f, 0.5f, 0.f),
      Vector3(-0.5f, -0.5f, 0.f),
      Vector3(0.5f, -0.5f, 0.f),
    };

    uint16 frameRectIndices[5]{0, 1, 3, 2, 0};
    if (!CreateMesh("FrameRect", frameRectVertices, sizeof(Vector3), 4, D3D11_USAGE_DEFAULT,
          D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, frameRectIndices, sizeof(uint16), 5,
          DXGI_FORMAT_R16_UINT))
        return false;

    Vector3 sphereVertices[36] = {};
    for (int32 i = 0; i < 36; ++i)
    {
        float angle = TAU<float> * i / 36.f;

        sphereVertices[i].x = cosf(angle) * 0.5f;
        sphereVertices[i].y = sinf(angle) * 0.5f;
    }

    uint16 sphereIndices[37] = {};
    for (int32 i = 0; i < 36; ++i)
    {
        sphereIndices[i] = i;
    }
    sphereIndices[36] = 0;
    if (!CreateMesh("FrameSphere", sphereVertices, sizeof(Vector3), 36, D3D11_USAGE_DEFAULT,
          D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, sphereIndices, sizeof(uint16), 37,
          DXGI_FORMAT_R16_UINT))
        return false;

    TextureVertex textureRectVertices[4]{TextureVertex(-0.5f, 0.5f, 0.f, 0.f, 0.f),
      TextureVertex(0.5f, 0.5f, 0.f, 1.f, 0.f), TextureVertex(-0.5f, -0.5f, 0.f, 0.f, 1.f),
      TextureVertex(0.5f, -0.5f, 0.f, 1.f, 1.f)};

    uint16 textureRectIndices[6]{0, 1, 3, 0, 3, 2};

    if (!CreateMesh("TextureRect", textureRectVertices, sizeof(TextureVertex), 4,
          D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, textureRectIndices,
          sizeof(uint16), 6, DXGI_FORMAT_R16_UINT))
        return false;

    return true;
}

void MeshManager::Destroy()
{
    for (auto& [_, mesh] : _meshes)
    {
        if (nullptr == mesh)
            continue;

        DESTROY(mesh)
    }

    _meshes.clear();
}

Ptr<Mesh> MeshManager::FindMesh(const std::string& name)
{
    auto itFinder = _meshFinder.find(name);
    if (_meshFinder.end() == itFinder)
        return nullptr;

    int32 id     = itFinder->second;
    auto  itMesh = _meshes.find(id);
    if (_meshes.end() == itMesh)
        return nullptr;

    return itMesh->second;
}

Ptr<Mesh> MeshManager::CreateMesh(const std::string& name,
  void*                                              vertexData,
  uint32                                             size,
  int32                                              count,
  D3D11_USAGE                                        vertexUsage,
  D3D11_PRIMITIVE_TOPOLOGY                           primitive,
  void*                                              indexData,
  uint32                                             indexSize,
  int32                                              indexCount,
  DXGI_FORMAT                                        fmt,
  D3D11_USAGE                                        indexUsage)
{
    Ptr<Mesh> meshFound = FindMesh(name);
    if (meshFound)
        return meshFound;

    Ptr<Mesh> mesh = New<Mesh>();
    if (!mesh->CreateMesh(vertexData, size, count, vertexUsage, primitive, indexData, indexSize,
          indexCount, fmt, indexUsage))
    {
        DESTROY(mesh)
        return nullptr;
    }

    mesh->SetName(name);
    mesh->SetID(_idCounter);
    _meshes[_idCounter] = mesh;
    _meshFinder[name]   = _idCounter;
    _idCounter++;

    return mesh;
}
