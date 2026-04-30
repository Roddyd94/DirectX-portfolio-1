#pragma once
#include "Shader.h"

class GraphicShader : public Shader
{
public:
    GraphicShader()           = default;
    ~GraphicShader() override = default;

private:
    std::vector<D3D11_INPUT_ELEMENT_DESC> _descs;
    ComPtr<ID3D11InputLayout>             _inputLayout = nullptr;

    ComPtr<ID3D11VertexShader> _vs     = nullptr;
    ComPtr<ID3DBlob>           _vsBlob = nullptr;
    ComPtr<ID3D11PixelShader>  _ps     = nullptr;
    ComPtr<ID3DBlob>           _psBlob = nullptr;

    uint32 _inputLayoutSize = 0;

public:
    bool Init() override;
    void Destroy() override;

    void SetShader() final;

    void AddInputLayoutDesc(const char* semetic,
      uint32                            sementicIndex,
      DXGI_FORMAT                       fmt,
      uint32                            inputSlot,
      uint32                            size,
      D3D11_INPUT_CLASSIFICATION        inputSlotClass,
      uint32                            instanceDataStepRate);

    bool CreateInputLayout();

    bool LoadVertexShader(const char* entryName, const char* fileName);
    bool LoadPixelShader(const char* entryName, const char* fileName);
};
