#pragma once
#include "Resource.h"

class ConstantBuffer : public Resource
{
public:
    ConstantBuffer()           = default;
    ~ConstantBuffer() override = default;

protected:
    ComPtr<ID3D11Buffer> _buffer      = nullptr;
    int32                _registerNum = -1;
    uint32               _size;
    uint8                _shaderType = 0;

public:
    virtual void Update() = 0;
    void         Destroy() override;

    bool Create(uint32 size, int32 registerNum, uint8 shaderType);
    void BindData(void* data);
};
