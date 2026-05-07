#pragma once
#include "Resource.h"

class StructureBuffer : public Resource
{
public:
    StructureBuffer()           = default;
    ~StructureBuffer() override = default;

protected:
    ComPtr<ID3D11Buffer>             _buffer;
    ComPtr<ID3D11ShaderResourceView> _srv;

    int32  _elementCount = 0;
    uint32 _size         = 0;
    uint32 _registerNum  = 0;
    uint8  _shaderType   = 0;

public:
    virtual void Update() = 0;
    void         Destroy() override;

    virtual int32 GetElementCount() const = 0;
    virtual void  Clear()                 = 0;

    bool Create(
      uint32 size, int32 elementCount, int32 registerNum, uint8 shaderType);
    void Bind();

protected:
    void SetData(void* data, int32 elementCount);
    void Resize(int32 elementCount);
};
