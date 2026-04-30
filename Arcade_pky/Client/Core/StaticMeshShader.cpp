#include "pch.h"

#include "StaticMeshShader.h"

bool StaticMeshShader::Init()
{
    if (!LoadVertexShader("StaticMeshVS", "StaticMesh.fx"))
        return false;

    if (!LoadPixelShader("StaticMeshPS", "StaticMesh.fx"))
        return false;

    AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
      D3D11_INPUT_PER_VERTEX_DATA, 0);
    AddInputLayoutDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8,
      D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}

void StaticMeshShader::Destroy() {}
