#include "pch.h"

#include "ConstantBuffer.h"

#include "DeviceManager.h"

#include "Rendering.h"
#include "Shader.h"

void ConstantBuffer::Update() {}

void ConstantBuffer::Destroy() {}

bool ConstantBuffer::Create(uint32 size, int32 registerNum, uint8 shaderType)
{
    _size        = size;
    _registerNum = registerNum;
    _shaderType  = shaderType;

    D3D11_BUFFER_DESC desc = {};
    desc.Usage             = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth         = _size;
    desc.BindFlags         = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, _buffer.GetAddressOf())))
        return false;

    return true;
}

void ConstantBuffer::SetData(void* data)
{
    D3D11_MAPPED_SUBRESOURCE map = {};

    CONTEXT->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
    memcpy(map.pData, data, _size);
    CONTEXT->Unmap(_buffer.Get(), 0);

    if (_shaderType & ShaderType::Vertex)
        CONTEXT->VSSetConstantBuffers(_registerNum, 1, _buffer.GetAddressOf());
    if (_shaderType & ShaderType::Pixel)
        CONTEXT->PSSetConstantBuffers(_registerNum, 1, _buffer.GetAddressOf());
}
