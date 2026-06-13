#include "pch.h"

#include "StructureBuffer.h"

#include "DeviceManager.h"

#include "Rendering.h"

void StructureBuffer::Destroy() {}

bool StructureBuffer::Create(uint32 size, int32 elementCount, int32 registerNum, uint8 shaderType)
{
    _size         = size;
    _elementCount = elementCount;
    _registerNum  = registerNum;
    _shaderType   = shaderType;

    if (_isByteBuffer)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.Usage             = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth         = _size * _elementCount;
        desc.BindFlags         = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags         = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

        if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, _buffer.GetAddressOf())))
            return false;

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format                          = DXGI_FORMAT_R32_TYPELESS;
        srvDesc.ViewDimension                   = D3D11_SRV_DIMENSION_BUFFEREX;
        srvDesc.BufferEx.FirstElement           = 0;
        srvDesc.BufferEx.NumElements            = _elementCount;
        srvDesc.BufferEx.Flags                  = D3D11_BUFFEREX_SRV_FLAG_RAW;

        if (FAILED(DEVICE->CreateShaderResourceView(_buffer.Get(), &srvDesc, _srv.GetAddressOf())))
            return false;
    }
    else
    {
        D3D11_BUFFER_DESC desc   = {};
        desc.Usage               = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth           = _size * _elementCount;
        desc.BindFlags           = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags           = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        desc.StructureByteStride = _size;

        if (FAILED(DEVICE->CreateBuffer(&desc, nullptr, _buffer.GetAddressOf())))
            return false;

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format                          = DXGI_FORMAT_UNKNOWN;
        srvDesc.ViewDimension                   = D3D11_SRV_DIMENSION_BUFFER;
        srvDesc.Buffer.FirstElement             = 0;
        srvDesc.Buffer.NumElements              = _elementCount;

        if (FAILED(DEVICE->CreateShaderResourceView(_buffer.Get(), &srvDesc, _srv.GetAddressOf())))
            return false;
    }

    return true;
}

void StructureBuffer::Bind()
{
    if (_shaderType & ShaderType::Vertex)
        CONTEXT->VSSetShaderResources(_registerNum, 1, _srv.GetAddressOf());
    if (_shaderType & ShaderType::Pixel)
        CONTEXT->PSSetShaderResources(_registerNum, 1, _srv.GetAddressOf());
}

void StructureBuffer::BindData(void* data, int32 elementCount)
{
    D3D11_MAPPED_SUBRESOURCE mapped = {};

    CONTEXT->Map(_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, data, _size * elementCount);
    CONTEXT->Unmap(_buffer.Get(), 0);
}

void StructureBuffer::Resize(int32 elementCount)
{
    if (_elementCount >= elementCount)
        return;

    _buffer.Reset();
    _srv.Reset();

    _elementCount = elementCount;

    Create(_size, _elementCount, _registerNum, _shaderType);
}
