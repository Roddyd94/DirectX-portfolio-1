#pragma once
#include "RenderState.h"

class BlendState : public RenderState
{
public:
    BlendState();
    ~BlendState() override;

protected:
    std::vector<D3D11_RENDER_TARGET_BLEND_DESC> _descs;

    float _blendFactor[4]         = {};
    float _previousBlendFactor[4] = {};
    UINT  _sampleMask             = 0xff'ff'ff'ff;
    UINT  _previousSampleMask     = 0;

public:
    void Destroy() override;

    void SetState() override;
    void ResetState() override;

public:
    bool CreateState(bool alphaToCoverage, bool independent);
    void SetBlendFactor(float r, float g, float b, float a);
    void SetSampleMask(UINT mask);
    void AddBlendDesc(bool enable,
      D3D11_BLEND          src,
      D3D11_BLEND          dest,
      D3D11_BLEND_OP       blendOp,
      D3D11_BLEND          srcAlpha,
      D3D11_BLEND          destAlpha,
      D3D11_BLEND_OP       alphaOp,
      UINT8                renderTargetWriteMask);
};
