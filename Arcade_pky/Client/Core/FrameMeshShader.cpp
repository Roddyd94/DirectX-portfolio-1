#include "pch.h"

#include "FrameMeshShader.h"

bool FrameMeshShader::Init()
{
    if (!LoadVertexShader("FrameMeshVS", "FrameMesh.fx"))
        return false;

    if (!LoadPixelShader("FrameMeshPS", "FrameMesh.fx"))
        return false;

    AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
      D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}
