#pragma once
#include "Object.h"

class StructureBuffer : public Object
{
public:
    StructureBuffer()           = default;
    ~StructureBuffer() override = default;

protected:
    ComPtr<ID3D11Buffer>             _buffer;
    ComPtr<ID3D11ShaderResourceView> _srv;

    uint32 _size         = 0;
    uint32 _elementCount = 0;
    uint32 _registerNum  = 0;
    uint8  _shaderType   = 0;

public:
    virtual void Update() = 0;
    void         Destroy() override;

    virtual uint32 GetElementCount() const = 0;
    virtual void  Clear()                 = 0;

    bool Create(
      uint32 size, uint32 elementCount, uint32 registerNum, uint8 shaderType);
    void Bind();

protected:
    void SetData(void* data, uint32 elementCount);
    void Resize(uint32 elementCount);
};
