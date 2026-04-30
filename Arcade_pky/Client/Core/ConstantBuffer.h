#pragma once
#include "Object.h"

class ConstantBuffer : public Object
{
public:
    ConstantBuffer()           = default;
    ~ConstantBuffer() override = default;

protected:
    ComPtr<ID3D11Buffer> _buffer = nullptr;
    uint32               _size;
    uint32               _registerNum = 0;
    uint8                _shaderType  = 0;

public:
    virtual void Update() = 0;
    void         Destroy() override;

    bool Create(uint32 size, uint32 registerNum, uint8 shaderType);
    void SetData(void* data);
};
