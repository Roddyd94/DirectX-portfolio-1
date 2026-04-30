#include "pch.h"

#include "ShaderManager.h"

#include "DeviceManager.h"

#include "ColorConstantBuffer.h"
#include "FrameMeshShader.h"
#include "MaterialConstantBuffer.h"
#include "StaticMeshShader.h"
#include "TransformConstantBuffer.h"

bool ShaderManager::Init()
{
    // TODO Create Shaders, Buffers

#pragma region SHADERS
    if (!CreateShader<StaticMeshShader>("StaticMeshShader"))
        return false;

    if (!CreateShader<FrameMeshShader>("FrameMeshShader"))
        return false;
#pragma endregion SHADERS

#pragma region VERTEX_CONSTANT_BUFFERS
    if (!CreateCBuffer<TransformConstantBuffer>("Transform",
          sizeof(TransformConstantBufferData), 0, ShaderType::Vertex))
        return false;
#pragma endregion VERTEX_CONSTANT_BUFFERS

#pragma region PIXEL_CONSTANT_BUFFERS
    if (!CreateCBuffer<MaterialConstantBuffer>(
          "Material", sizeof(MaterialConstantBufferData), 0, ShaderType::Pixel))
        return false;

    if (!CreateCBuffer<ColorConstantBuffer>(
          "Color", sizeof(ColorConstantBufferData), 0, ShaderType::Pixel))
        return false;
#pragma endregion PIXEL_CONSTANT_BUFFERS

    CreateSamplers();

    return true;
}

void ShaderManager::Destroy()
{
    for (auto& [_, shader] : _shaders)
        DESTROY(shader)

    for (auto& [_, constantBuffer] : _constantBuffers)
        DESTROY(constantBuffer)

    // for (auto& [_, structBuffer] : _structBuffers)
    //     DESTROY(structBuffer)

    _shaders.clear();
    _constantBuffers.clear();
    //_structBuffers.clear();
}

void ShaderManager::SetSampler(uint8 type)
{
    CONTEXT->PSSetSamplers(0, 1, _samplers[type].GetAddressOf());
}

void ShaderManager::CreateSamplers()
{
    D3D11_SAMPLER_DESC desc = {};
    desc.AddressU           = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV           = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW           = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.ComparisonFunc     = D3D11_COMPARISON_NEVER;

    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    DEVICE->CreateSamplerState(
      &desc, _samplers[SamplerType::Point].GetAddressOf());

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    DEVICE->CreateSamplerState(
      &desc, _samplers[SamplerType::Linear].GetAddressOf());

    desc.Filter        = D3D11_FILTER_ANISOTROPIC;
    desc.MaxAnisotropy = 16;

    DEVICE->CreateSamplerState(
      &desc, _samplers[SamplerType::Anisotropic].GetAddressOf());
}
