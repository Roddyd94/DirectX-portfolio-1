#include "pch.h"

#include "RenderStateManager.h"

#include "BlendState.h"
#include "DepthStencilState.h"
#include "RenderState.h"

bool RenderStateManager::Init()
{
    D3D11_RENDER_TARGET_BLEND_DESC opaque = {};

    opaque.BlendEnable           = TRUE;
    opaque.SrcBlend              = D3D11_BLEND_ONE;
    opaque.DestBlend             = D3D11_BLEND_ZERO;
    opaque.BlendOp               = D3D11_BLEND_OP_ADD;
    opaque.SrcBlendAlpha         = D3D11_BLEND_ONE;
    opaque.DestBlendAlpha        = D3D11_BLEND_ZERO;
    opaque.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    opaque.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_RENDER_TARGET_BLEND_DESC alphaBlend = {};

    alphaBlend.BlendEnable           = TRUE;
    alphaBlend.SrcBlend              = D3D11_BLEND_SRC_ALPHA;
    alphaBlend.DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    alphaBlend.BlendOp               = D3D11_BLEND_OP_ADD;
    alphaBlend.SrcBlendAlpha         = D3D11_BLEND_ONE;
    alphaBlend.DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
    alphaBlend.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    alphaBlend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_RENDER_TARGET_BLEND_DESC premultipliedAlpha = {};

    premultipliedAlpha.BlendEnable           = TRUE;
    premultipliedAlpha.SrcBlend              = D3D11_BLEND_ONE;
    premultipliedAlpha.DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
    premultipliedAlpha.BlendOp               = D3D11_BLEND_OP_ADD;
    premultipliedAlpha.SrcBlendAlpha         = D3D11_BLEND_ONE;
    premultipliedAlpha.DestBlendAlpha        = D3D11_BLEND_INV_SRC_ALPHA;
    premultipliedAlpha.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    premultipliedAlpha.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_RENDER_TARGET_BLEND_DESC additive = {};

    additive.BlendEnable           = TRUE;
    additive.SrcBlend              = D3D11_BLEND_ONE;
    additive.DestBlend             = D3D11_BLEND_ONE;
    additive.BlendOp               = D3D11_BLEND_OP_ADD;
    additive.SrcBlendAlpha         = D3D11_BLEND_ONE;
    additive.DestBlendAlpha        = D3D11_BLEND_ONE;
    additive.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    additive.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_RENDER_TARGET_BLEND_DESC multiply = {};

    multiply.BlendEnable           = TRUE;
    multiply.SrcBlend              = D3D11_BLEND_ZERO;
    multiply.DestBlend             = D3D11_BLEND_SRC_COLOR;
    multiply.BlendOp               = D3D11_BLEND_OP_ADD;
    multiply.SrcBlendAlpha         = D3D11_BLEND_ONE;
    multiply.DestBlendAlpha        = D3D11_BLEND_ONE;
    multiply.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    multiply.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    AddBlendDesc("Opaque", opaque);
    CreateState("Opaque", false, false, opaque);

    AddBlendDesc("AlphaBlend", alphaBlend);
    CreateState("AlphaBlend", true, false, alphaBlend);

    AddBlendDesc("PMA", premultipliedAlpha);
    CreateState("PMA", false, false, premultipliedAlpha);

    AddBlendDesc("Additive", additive);
    CreateState("Additive", false, false, additive);

    AddBlendDesc("Multiply", multiply);
    CreateState("Multiply", false, false, multiply);

    CreateDepthStencilState("DepthStencil", false);

    return true;
}

void RenderStateManager::Destroy()
{
    for (auto& [_, renderState] : _renderStates)
        DESTROY(renderState)
}

void RenderStateManager::SetBlendFactor(const std::string& name, float r, float g, float b, float a)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state               = New<BlendState>();
        _renderStates[name] = state;
    }

    state->SetBlendFactor(r, g, b, a);
}

void RenderStateManager::SetSampleMask(const std::string& name, UINT mask)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state               = New<BlendState>();
        _renderStates[name] = state;
    }

    state->SetSampleMask(mask);
}

void RenderStateManager::AddBlendDesc(const std::string& name,
  bool                                                   enable,
  D3D11_BLEND                                            src,
  D3D11_BLEND                                            dest,
  D3D11_BLEND_OP                                         blendop,
  D3D11_BLEND                                            srcAlpha,
  D3D11_BLEND                                            destAlpha,
  D3D11_BLEND_OP                                         alphaOp,
  UINT8                                                  renderTargetWriteMask)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state               = New<BlendState>();
        _renderStates[name] = state;
    }

    state->AddBlendDesc(
      enable, src, dest, blendop, srcAlpha, destAlpha, alphaOp, renderTargetWriteMask);
}

void RenderStateManager::AddBlendDesc(
  const std::string& name, D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state               = New<BlendState>();
        _renderStates[name] = state;
    }

    state->AddBlendDesc(rtBlendDesc);
}

bool RenderStateManager::CreateState(const std::string& name,
  bool                                                  alphaToCoverage,
  bool                                                  independent,
  D3D11_RENDER_TARGET_BLEND_DESC                        rtBlendDesc)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state               = New<BlendState>();
        _renderStates[name] = state;
    }

    if (!state->CreateState(alphaToCoverage, independent, rtBlendDesc))
    {
        DESTROY(state);
        return false;
    }

    return true;
}

bool RenderStateManager::CreateDepthStencilState(const std::string& name,
  bool                                                              depthEnable,
  D3D11_DEPTH_WRITE_MASK                                            depthWriteMask,
  D3D11_COMPARISON_FUNC                                             depthFunc,
  bool                                                              stencilEnable,
  UINT8                                                             stencilReadMask,
  UINT8                                                             stencilWriteMask,
  D3D11_DEPTH_STENCILOP_DESC                                        frontFace,
  D3D11_DEPTH_STENCILOP_DESC                                        backFace)
{
    Ptr<DepthStencilState> state = FindRenderState<DepthStencilState>(name);
    if (nullptr != state)
        return false;

    state = New<DepthStencilState>();
    if (!state->CreateState(depthEnable, depthWriteMask, depthFunc, stencilEnable, stencilReadMask,
          stencilWriteMask, frontFace, backFace))
    {
        DESTROY(state)
        return false;
    }

    _renderStates[name] = state;
    return true;
}
