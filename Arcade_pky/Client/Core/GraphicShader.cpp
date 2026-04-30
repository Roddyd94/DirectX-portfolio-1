#include "pch.h"

#include "GraphicShader.h"

#include "DeviceManager.h"
#include "DirectoryManager.h"
#include "Common/LogManager.h"

bool GraphicShader::Init()
{
    return false;
}

void GraphicShader::Destroy() {}

void GraphicShader::SetShader()
{
    CONTEXT->VSSetShader(_vs.Get(), nullptr, 0);
    CONTEXT->PSSetShader(_ps.Get(), nullptr, 0);

    CONTEXT->IASetInputLayout(_inputLayout.Get());
}

void GraphicShader::AddInputLayoutDesc(const char* semetic,
  uint32                                           sementicIndex,
  DXGI_FORMAT                                      fmt,
  uint32                                           inputSlot,
  uint32                                           size,
  D3D11_INPUT_CLASSIFICATION                       inputSlotClass,
  uint32                                           instanceDataStepRate)
{
    D3D11_INPUT_ELEMENT_DESC desc = {};

    desc.SemanticName         = semetic;
    desc.SemanticIndex        = sementicIndex;
    desc.Format               = fmt;
    desc.InputSlot            = inputSlot;
    desc.InputSlotClass       = inputSlotClass;
    desc.InstanceDataStepRate = instanceDataStepRate;
    desc.AlignedByteOffset    = _inputLayoutSize;

    _inputLayoutSize += size;

    _descs.push_back(desc);
}

bool GraphicShader::CreateInputLayout()
{
    if (FAILED(DEVICE->CreateInputLayout(&_descs[0], (UINT)_descs.size(),
          _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(),
          &_inputLayout)))
        return false;

    return true;
}

bool GraphicShader::LoadVertexShader(
  const char* entryName, const char* fileName)
{
    auto hlslPath = DirectoryManager::Instance().GetCachePath("HLSL");
    if (!hlslPath.has_value())
        return false;

    std::filesystem::path fullPath;
    if (!DirectoryManager::Instance().GetFile(
          hlslPath.value(), fileName, OUT fullPath))
        return false;

    uint32 flag = 0;
#ifdef _DEBUG
    flag = D3DCOMPILE_DEBUG;
#endif

    ID3D10Blob* errorBlob = nullptr;
    if (FAILED(D3DCompileFromFile(fullPath.c_str(), nullptr,
          D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "vs_5_0", flag, 0,
          _vsBlob.GetAddressOf(), &errorBlob)))
    {
#ifdef _DEBUG
        char errorText[255] = {};
        strcpy_s(errorText, (const char*)errorBlob->GetBufferPointer());
        LogManager::Instance().Debug(errorText);
#endif
        return false;
    }

    if (FAILED(DEVICE->CreateVertexShader(_vsBlob->GetBufferPointer(),
          _vsBlob->GetBufferSize(), nullptr, _vs.GetAddressOf())))
        return false;

    return true;
}

bool GraphicShader::LoadPixelShader(const char* entryName, const char* fileName)
{
    auto hlslPath = DirectoryManager::Instance().GetCachePath("HLSL");
    if (!hlslPath.has_value())
        return false;

    std::filesystem::path fullPath;
    if (!DirectoryManager::Instance().GetFile(
          hlslPath.value(), fileName, OUT fullPath))
        return false;

    uint32 flag = 0;
#ifdef _DEBUG
    flag = D3DCOMPILE_DEBUG;
#endif

    ID3D10Blob* errorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(fullPath.c_str(), nullptr,
          D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "ps_5_0", flag, 0,
          _psBlob.GetAddressOf(), &errorBlob)))
    {
#ifdef _DEBUG
        char errorText[255] = {};
        strcpy_s(errorText, (const char*)errorBlob->GetBufferPointer());
        LogManager::Instance().Debug(errorText);
#endif
        return false;
    }

    if (FAILED(DEVICE->CreatePixelShader(_psBlob->GetBufferPointer(),
          _psBlob->GetBufferSize(), nullptr, _ps.GetAddressOf())))
        return false;

    return true;
}
