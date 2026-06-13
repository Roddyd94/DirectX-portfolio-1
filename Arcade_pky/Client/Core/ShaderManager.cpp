#include "pch.h"

#include "ShaderManager.h"

#include "DeviceManager.h"

#include "ColorConstantBuffer.h"
#include "FrameMeshShader.h"
#include "MaterialConstantBuffer.h"
#include "PaletteStructureBuffer.h"
#include "StaticMeshShader.h"
#include "TransformConstantBuffer.h"

#ifdef _HAS_TILEMAP_MODULE
#include "Tilemap/IndexedTileInstanceShader.h"
#include "Tilemap/IndexedTileStructureBuffer.h"
#include "Tilemap/TileInstanceShader.h"
#include "Tilemap/TileOutlineInstanceShader.h"
#include "Tilemap/TileOutlineStructureBuffer.h"
#include "Tilemap/TileStructureBuffer.h"
#endif // _HAS_TILEMAP_MODULE

#include "IndexedTextureBuffer.h"
#include "IndexedTextureInfoConstantBuffer.h"
#include "Animation/Animation2DConstantBuffer.h"
#include "Animation/Animation2DStructureBuffer.h"
#include "Animation/SpriteInstanceShader.h"
#include "Animation/SpriteShader.h"
#include "Snowbros/IndexedAnimation2DConstantBuffer.h"
#include "Snowbros/IndexedAnimation2DStructureBuffer.h"
#include "Snowbros/IndexedSpriteInstanceShader.h"
#include "Snowbros/IndexedSpriteShader.h"

bool ShaderManager::Init()
{
#pragma region SHADERS
    if (!CreateShader<StaticMeshShader>("StaticMeshShader"))
        return false;

    if (!CreateShader<FrameMeshShader>("FrameMeshShader"))
        return false;

    if (!CreateShader<TileOutlineInstanceShader>("TileOutlineInstanceShader"))
        return false;

#ifdef _HAS_TILEMAP_MODULE
    if (!CreateShader<TileInstanceShader>("TileInstanceShader"))
        return false;

    if (!CreateShader<IndexedTileInstanceShader>("IndexedTileInstanceShader"))
        return false;
#endif // _HAS_TILEMAP_MODULE

    if (!CreateShader<SpriteShader>("SpriteShader"))
        return false;

    if (!CreateShader<IndexedSpriteShader>("IndexedSpriteShader"))
        return false;

    if (!CreateShader<SpriteInstanceShader>("SpriteInstanceShader"))
        return false;

    if (!CreateShader<IndexedSpriteInstanceShader>("IndexedSpriteInstanceShader"))
        return false;
#pragma endregion SHADERS

#pragma region VERTEX_CONSTANT_BUFFERS
    if (!CreateConstantBuffer<TransformConstantBuffer>(
          "Transform", sizeof(TransformConstantBufferData), 0, ShaderType::Vertex))
        return false;

    if (!CreateConstantBuffer<Animation2DConstantBuffer>(
          "Animation2D", sizeof(Animation2DConstantBufferData), 1, ShaderType::Vertex))
        return false;

    if (!CreateConstantBuffer<IndexedAnimation2DConstantBuffer>("IndexedAnimation2D",
          sizeof(IndexedAnimation2DConstantBufferData), 1, ShaderType::Vertex))
        return false;
#pragma endregion VERTEX_CONSTANT_BUFFERS

#pragma region PIXEL_CONSTANT_BUFFERS
    if (!CreateConstantBuffer<ColorConstantBuffer>(
          "Color", sizeof(ColorConstantBufferData), 0, ShaderType::Pixel))
        return false;

    if (!CreateConstantBuffer<MaterialConstantBuffer>(
          "Material", sizeof(MaterialConstantBufferData), 0, ShaderType::Pixel))
        return false;

    if (!CreateConstantBuffer<IndexedTextureInfoConstantBuffer>(
          "IndexedTextureInfo", sizeof(IndexedTextureInfoConstantBufferData), 0, ShaderType::Pixel))
        return false;
#pragma endregion PIXEL_CONSTANT_BUFFERS

#pragma region STRUCTURE_BUFFERS
    if (!CreateStructureBuffer<TileOutlineStructureBuffer>(
          "SphereMesh", sizeof(TileOutlineInstanceData), 128, 1, ShaderType::Vertex))
        return false;

    if (!CreateStructureBuffer<TileOutlineStructureBuffer>(
          "RectMesh", sizeof(TileOutlineInstanceData), 128, 1, ShaderType::Vertex))
        return false;

    if (!CreateStructureBuffer<IndexedTileStructureBuffer>(
          "UI", sizeof(IndexedTileInstanceData), 128, 1, ShaderType::Vertex))
        return false;

#ifdef _HAS_TILEMAP_MODULE
    if (!CreateStructureBuffer<TileStructureBuffer>(
          "Tile", sizeof(TileInstanceData), 128, 1, ShaderType::Vertex))
        return false;

    if (!CreateStructureBuffer<TileOutlineStructureBuffer>(
          "TileOutline", sizeof(TileOutlineInstanceData), 128, 1, ShaderType::Vertex))
        return false;

    if (!CreateStructureBuffer<IndexedTileStructureBuffer>(
          "IndexedTile", sizeof(IndexedTileInstanceData), 128, 1, ShaderType::Vertex))
        return false;
#endif // _HAS_TILEMAP_MODULE

    if (!CreateStructureBuffer<Animation2DStructureBuffer>(
          "Animation2DInstance", sizeof(Animation2DInstanceData), 128, 1, ShaderType::Vertex))
        return false;

    if (!CreateStructureBuffer<IndexedAnimation2DStructureBuffer>("IndexedAnimation2DInstance",
          sizeof(IndexedAnimation2DInstanceData), 128, 1, ShaderType::Vertex))
        return false;

    if (!CreateStructureBuffer<IndexedTextureBuffer>(
          "IndexedTexture", sizeof(int32), 128, 0, ShaderType::Pixel))
        return false;

    if (!CreateStructureBuffer<PaletteStructureBuffer>(
          "Palette", sizeof(Vector4), 128, 1, ShaderType::Pixel))
        return false;

#pragma endregion STRUCTURE_BUFFERS

    CreateSamplers();

    return true;
}

void ShaderManager::Destroy()
{
    for (auto& [_, shader] : _shaders)
        DESTROY(shader)

    for (auto& [_, constantBuffer] : _constantBuffers)
        DESTROY(constantBuffer)

    for (auto& [_, structBuffer] : _structureBuffers)
        DESTROY(structBuffer)

    _shaders.clear();
    _constantBuffers.clear();
    _structureBuffers.clear();
}

void ShaderManager::SetSampler(uint8 type)
{
    CONTEXT->PSSetSamplers(0, 1, _samplers[type].GetAddressOf());
}

void ShaderManager::CreateSamplers()
{
    D3D11_SAMPLER_DESC desc = {};
    desc.AddressU           = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressV           = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.AddressW           = D3D11_TEXTURE_ADDRESS_CLAMP;
    desc.ComparisonFunc     = D3D11_COMPARISON_NEVER;

    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    DEVICE->CreateSamplerState(&desc, _samplers[SamplerType::Point].GetAddressOf());

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    DEVICE->CreateSamplerState(&desc, _samplers[SamplerType::Linear].GetAddressOf());

    desc.Filter        = D3D11_FILTER_ANISOTROPIC;
    desc.MaxAnisotropy = 16;

    DEVICE->CreateSamplerState(&desc, _samplers[SamplerType::Anisotropic].GetAddressOf());
}
