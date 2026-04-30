#include "pch.h"

#include "DepthStencilState.h"

#include "DeviceManager.h"

DepthStencilState::DepthStencilState()
    : RenderState(RenderStateType::DepthStencil)
{
}

DepthStencilState::~DepthStencilState() {}

void DepthStencilState::Destroy() {}

void DepthStencilState::SetState()
{
    CONTEXT->OMGetDepthStencilState(
      (ID3D11DepthStencilState**)_prevState.GetAddressOf(),
      &_previousStencilRef);
    CONTEXT->OMSetDepthStencilState(
      (ID3D11DepthStencilState*)_state.Get(), _stencilRef);
}

void DepthStencilState::ResetState()
{
    CONTEXT->OMSetDepthStencilState(
      (ID3D11DepthStencilState*)_prevState.Get(), _previousStencilRef);
    _prevState = nullptr;
}

bool DepthStencilState::CreateState(bool depthEnable,
  D3D11_DEPTH_WRITE_MASK                 depthWriteMask,
  D3D11_COMPARISON_FUNC                  depthFunc,
  bool                                   stencilEnable,
  UINT8                                  stencilReadMask,
  UINT8                                  stencilWriteMask,
  D3D11_DEPTH_STENCILOP_DESC             frontFace,
  D3D11_DEPTH_STENCILOP_DESC             backFace)
{
    D3D11_DEPTH_STENCIL_DESC desc = {};

    desc.DepthEnable    = depthEnable;
    desc.DepthWriteMask = depthWriteMask;
    desc.DepthFunc      = depthFunc;

    desc.StencilEnable    = stencilEnable;
    desc.StencilReadMask  = stencilReadMask;
    desc.StencilWriteMask = stencilWriteMask;

    desc.FrontFace = frontFace;
    desc.BackFace  = backFace;

    if (FAILED(DEVICE->CreateDepthStencilState(
          &desc, (ID3D11DepthStencilState**)_state.GetAddressOf())))
        return false;

    return true;
}
