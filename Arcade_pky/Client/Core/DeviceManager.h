#pragma once
#include "Singleton.h"
#include "Core/Info.h"

class DeviceManager : public Singleton<DeviceManager>
{
    DECLARE_SINGLETON(DeviceManager)
private:
    HWND _hWnd = nullptr;

    ComPtr<ID3D11Device>        _device     = nullptr;
    ComPtr<ID3D11DeviceContext> _context    = nullptr;
    ComPtr<IDXGISwapChain>      _swapChain  = nullptr;
    ComPtr<ID3D11Texture2D>     _backBuffer = nullptr;

    std::map<std::string, ComPtr<ID3D11Texture2D>>          _textures;
    std::map<std::string, ComPtr<ID3D11RenderTargetView>>   _rtvs;
    std::map<std::string, ComPtr<ID3D11ShaderResourceView>> _srvs;
    std::map<std::string, ComPtr<ID3D11DepthStencilView>>   _dsvs;
    std::map<std::string, ComPtr<ID2D1RenderTarget>>        _renderTargets2D;
    ComPtr<ID2D1Factory> _factory2D = nullptr;

    ComPtr<ID3D11RenderTargetView> _rtvDefault            = nullptr;
    ComPtr<ID3D11DepthStencilView> _dsvDefault            = nullptr;
    ComPtr<ID2D1RenderTarget>      _renderTargetDefault2D = nullptr;

    Resolution _resolution = {};
    bool       _isWindowed = false;

public:
    bool Init(HWND hWnd, uint32 width, uint32 height, bool isWindowed);
    void Destroy();

public:
    ComPtr<ID3D11Device>        GetDevice() const { return _device; }
    ComPtr<ID3D11DeviceContext> GetContext() const { return _context; }
    ComPtr<ID3D11Texture2D>     GetBackBuffer() const { return _backBuffer; }

    ComPtr<ID3D11RenderTargetView> GetDefaultRenderTargetView() const
    {
        return _rtvDefault;
    }
    ComPtr<ID3D11DepthStencilView> GetDefaultDepthStencilView() const
    {
        return _dsvDefault;
    }
    ComPtr<ID2D1RenderTarget> GetDefaultRenderTarget2D() const
    {
        return _renderTargetDefault2D;
    }

    bool              GetWindowMode() { return _isWindowed; }
    const Resolution& GetResolution() const { return _resolution; }
    const Vector2     GetResolutionRate() const;

    ComPtr<ID3D11Texture2D>        FindTexture2D(const std::string& name) const;
    ComPtr<ID3D11RenderTargetView> FindRenderTargetView(
      const std::string& name) const;
    ComPtr<ID3D11ShaderResourceView> FindShaderResourceView(
      const std::string& name) const;
    ComPtr<ID3D11DepthStencilView> FindDepthStencilView(
      const std::string& name) const;
    ComPtr<ID2D1RenderTarget> FindRenderTarget2D(const std::string& name) const;

public:
    ComPtr<ID3D11Texture2D> CreateTexture2D(
      const std::string& name, D3D11_TEXTURE2D_DESC* desc);
    ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(
      const std::string&             name,
      ComPtr<ID3D11Texture2D>        texture = nullptr,
      D3D11_RENDER_TARGET_VIEW_DESC* desc    = nullptr);
    ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(
      const std::string&             name,
      const std::string&             textureName,
      D3D11_RENDER_TARGET_VIEW_DESC* desc = nullptr);
    ComPtr<ID3D11ShaderResourceView> CreateShaderResourceView(
      const std::string&               name,
      ComPtr<ID3D11Texture2D>          texture,
      D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr);
    ComPtr<ID3D11ShaderResourceView> CreateShaderResourceView(
      const std::string&               name,
      const std::string&               textureName,
      D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr);
    ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(
      const std::string&             name,
      ComPtr<ID3D11Texture2D>        texture,
      D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr);
    ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(
      const std::string&             name,
      const std::string&             textureName,
      D3D11_DEPTH_STENCIL_VIEW_DESC* desc = nullptr);
    ComPtr<ID2D1RenderTarget> CreateRenderTarget2D(
      const std::string& name, ComPtr<IDXGISurface> surface = nullptr);
    void SetTarget();
    void Render();
    void ClearBackBuffer(float clearColor[4]);
    void ClearDepthStencil(float depth, uint8 stencil);
};

#define DEVICE  DeviceManager::Instance().GetDevice()
#define CONTEXT DeviceManager::Instance().GetContext()

#define RENDER_TARGET_2D DeviceManager::Instance().GetContext()
