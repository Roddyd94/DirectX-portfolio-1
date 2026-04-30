#pragma once
#include "Object.h"

enum class RenderStateType
{
    Blend,
    DepthStencil,
    End
};

class RenderState : public Object
{
public:
    RenderState() = default;
    RenderState(RenderStateType type);
    virtual ~RenderState() = default;

protected:
    ComPtr<ID3D11DeviceChild> _state;
    ComPtr<ID3D11DeviceChild> _prevState;
    RenderStateType           _type = RenderStateType::End;

public:
    virtual void SetState()   = 0;
    virtual void ResetState() = 0;
};
