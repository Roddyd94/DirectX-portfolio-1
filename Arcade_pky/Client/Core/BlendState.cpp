#include "pch.h"

#include "BlendState.h"

#include "DeviceManager.h"

BlendState::BlendState() : RenderState(RenderStateType::Blend) {}

BlendState::~BlendState() {}

void BlendState::SetState()
{
    CONTEXT->OMGetBlendState(
      (ID3D11BlendState**)_prevState.GetAddressOf(), _previousBlendFactor, &_previousSampleMask);
    CONTEXT->OMSetBlendState((ID3D11BlendState*)_state.Get(), _blendFactor, _sampleMask);
}

void BlendState::ResetState()
{
    CONTEXT->OMSetBlendState(
      (ID3D11BlendState*)_prevState.Get(), _previousBlendFactor, _previousSampleMask);

    _prevState = nullptr;
}

void BlendState::Destroy() {}

bool BlendState::CreateState(
  bool alphaToCoverage, bool independent, D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc)
{
    if (_descs.empty())
        return false;

    D3D11_BLEND_DESC desc = {};

    desc.AlphaToCoverageEnable  = alphaToCoverage;
    desc.IndependentBlendEnable = independent;

    for (int i = 0; i < 8; ++i)
        desc.RenderTarget[i] = rtBlendDesc;

    memcpy(desc.RenderTarget, &_descs[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * _descs.size());

    if (FAILED(DEVICE->CreateBlendState(&desc, (ID3D11BlendState**)_state.GetAddressOf())))
        return false;

    return true;
}

void BlendState::SetBlendFactor(float r, float g, float b, float a)
{
    _blendFactor[0] = r;
    _blendFactor[1] = g;
    _blendFactor[2] = b;
    _blendFactor[3] = a;
}

void BlendState::SetSampleMask(UINT mask)
{
    _sampleMask = mask;
}

void BlendState::AddBlendDesc(D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc)
{
    _descs.emplace_back(rtBlendDesc);
}

void BlendState::AddBlendDesc(bool enable,
  D3D11_BLEND                      src,
  D3D11_BLEND                      dest,
  D3D11_BLEND_OP                   blendOp,
  D3D11_BLEND                      srcAlpha,
  D3D11_BLEND                      destAlpha,
  D3D11_BLEND_OP                   alphaOp,
  UINT8                            renderTargetWriteMask)
{
    D3D11_RENDER_TARGET_BLEND_DESC desc = {};

    desc.BlendEnable           = enable;
    desc.SrcBlend              = src;
    desc.DestBlend             = dest;
    desc.BlendOp               = blendOp;
    desc.SrcBlendAlpha         = srcAlpha;
    desc.DestBlendAlpha        = destAlpha;
    desc.BlendOpAlpha          = alphaOp;
    desc.RenderTargetWriteMask = renderTargetWriteMask;

    _descs.emplace_back(desc);
}
