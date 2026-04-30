#include "pch.h"

#include "RenderStateManager.h"

#include "BlendState.h"
#include "DepthStencilState.h"
#include "RenderState.h"

bool RenderStateManager::Init()
{
    AddBlendDesc("AlphaBlend", true, D3D11_BLEND_SRC_ALPHA,
      D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE,
      D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, D3D11_COLOR_WRITE_ENABLE_ALL);

    CreateState("AlphaBlend", true, false);

    CreateDepthStencilState("DepthStencil", false);

    return true;
}

void RenderStateManager::Destroy()
{
    for (auto& [_, renderState] : _renderStates)
        DESTROY(renderState)
}

void RenderStateManager::SetBlendFactor(
  const std::string& name, float r, float g, float b, float a)
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

    state->AddBlendDesc(enable, src, dest, blendop, srcAlpha, destAlpha,
      alphaOp, renderTargetWriteMask);
}

bool RenderStateManager::CreateState(
  const std::string& name, bool alphaToCoverage, bool independent)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state               = New<BlendState>();
        _renderStates[name] = state;
    }

    if (!state->CreateState(alphaToCoverage, independent))
    {
        DESTROY(state);
        return false;
    }

    return true;
}

bool RenderStateManager::CreateDepthStencilState(const std::string& name,
  bool                                                              depthEnable,
  D3D11_DEPTH_WRITE_MASK     depthWriteMask,
  D3D11_COMPARISON_FUNC      depthFunc,
  bool                       stencilEnable,
  UINT8                      stencilReadMask,
  UINT8                      stencilWriteMask,
  D3D11_DEPTH_STENCILOP_DESC frontFace,
  D3D11_DEPTH_STENCILOP_DESC backFace)
{
    Ptr<DepthStencilState> state = FindRenderState<DepthStencilState>(name);
    if (nullptr != state)
        return false;

    state = New<DepthStencilState>();
    if (!state->CreateState(depthEnable, depthWriteMask, depthFunc,
          stencilEnable, stencilReadMask, stencilWriteMask, frontFace,
          backFace))
    {
        DESTROY(state)
        return false;
    }

    _renderStates[name] = state;
    return true;
}
