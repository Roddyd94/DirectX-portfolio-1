#include "pch.h"

#include "TileOutlineInstanceShader.h"

bool TileOutlineInstanceShader::Init()
{
    if (!LoadVertexShader("TileOutlineInstanceVS", "TileOutlineInstance.fx"))
        return false;

    if (!LoadPixelShader("TileOutlineInstancePS", "TileOutlineInstance.fx"))
        return false;

    AddInputLayoutDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
      D3D11_INPUT_PER_VERTEX_DATA, 0);

    if (!CreateInputLayout())
        return false;

    return true;
}
