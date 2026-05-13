#include "pch.h"

#include "DeviceManager.h"

#include "Common/LogManager.h"

bool DeviceManager::Init(HWND hWnd, uint32 width, uint32 height, bool isWindowed)
{
    _hWnd              = hWnd;
    _resolution.width  = width;
    _resolution.height = height;
    _isWindowed        = isWindowed;

    uint32 flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

    D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL levelResult;

    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, &level, 1,
          D3D11_SDK_VERSION, _device.GetAddressOf(), &levelResult, _context.GetAddressOf())))
        return false;

    uint32 sampleCount   = 4;
    uint32 qualityLevels = 0;
    _device->CheckMultisampleQualityLevels(
      DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, sampleCount, &qualityLevels);

    if (qualityLevels <= 0)
        sampleCount = 1;

    DXGI_SWAP_CHAIN_DESC swapDesc               = {};
    swapDesc.BufferDesc.Width                   = width;
    swapDesc.BufferDesc.Height                  = height;
    swapDesc.BufferDesc.RefreshRate.Numerator   = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    swapDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.SampleDesc.Quality                 = 0;
    swapDesc.SampleDesc.Count                   = sampleCount;
    swapDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount                        = 2;
    swapDesc.OutputWindow                       = _hWnd;
    swapDesc.Windowed                           = _isWindowed;
    swapDesc.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;

    ComPtr<IDXGIDevice> dxgiDevice = nullptr;
    _device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());

    ComPtr<IDXGIAdapter> adapter = nullptr;
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)adapter.GetAddressOf());

    ComPtr<IDXGIFactory> factory = nullptr;
    adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());

    if (FAILED(factory->CreateSwapChain(_device.Get(), &swapDesc, _swapChain.GetAddressOf())))
        return false;

    if (FAILED(
          _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)_backBuffer.GetAddressOf())))
        return false;

    _rtvDefault = CreateRenderTargetView("Default");
    if (nullptr == _rtvDefault)
        return false;

    D3D11_TEXTURE2D_DESC dsvTexDesc = {};
    dsvTexDesc.Width                = width;
    dsvTexDesc.Height               = height;
    dsvTexDesc.MipLevels            = 1;
    dsvTexDesc.ArraySize            = 1;
    dsvTexDesc.Format               = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvTexDesc.SampleDesc.Count     = sampleCount;
    dsvTexDesc.SampleDesc.Quality   = 0;
    dsvTexDesc.Usage                = D3D11_USAGE_DEFAULT;
    dsvTexDesc.BindFlags            = D3D11_BIND_DEPTH_STENCIL;

    ComPtr<ID3D11Texture2D> dsvTexture = CreateTexture2D("Default_DSV", &dsvTexDesc);
    if (nullptr == dsvTexture)
        return false;

    _dsvDefault = CreateDepthStencilView("Default", dsvTexture, nullptr);
    if (nullptr == _dsvDefault)
        return false;

    D3D11_VIEWPORT vp = {};

    vp.Width    = static_cast<float>(width);
    vp.Height   = static_cast<float>(height);
    vp.MaxDepth = 1;

    _context->RSSetViewports(1, &vp);

    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1),
          (void**)_factory2D.GetAddressOf())))
        return false;

    // RENDERDOC FAILS HERE
    if (FAILED(_factory2D->CreateDevice(dxgiDevice.Get(), _device2D.GetAddressOf())))
        return false;
    
    if (FAILED(_device2D->CreateDeviceContext(
          D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
          _context2D.GetAddressOf())))
        return false;

    ComPtr<IDXGISurface> dxgiSurface;
    if (FAILED(_backBuffer->QueryInterface(IID_PPV_ARGS(&dxgiSurface))))
        return false;

    _bitmapDefault = CreateBitmap("Default", dxgiSurface);
    if (nullptr == _bitmapDefault)
        return false;

    return true;
}

void DeviceManager::Destroy() {}

Vector2 DeviceManager::GetResolutionRate() const
{
    RECT windowRC;
    GetClientRect(_hWnd, &windowRC);

    float width  = static_cast<float>(windowRC.right - windowRC.left);
    float height = static_cast<float>(windowRC.bottom - windowRC.top);

    return Vector2(_resolution.width / width, _resolution.height / height);
}

ComPtr<ID3D11Texture2D> DeviceManager::FindTexture2D(const std::string& name) const
{
    if (auto it = _textures.find(name); _textures.end() != it)
        return it->second;

    return nullptr;
}

ComPtr<ID3D11RenderTargetView> DeviceManager::FindRenderTargetView(const std::string& name) const
{
    if (auto it = _rtvs.find(name); _rtvs.end() != it)
        return it->second;

    return nullptr;
}

ComPtr<ID3D11ShaderResourceView> DeviceManager::FindShaderResourceView(
  const std::string& name) const
{
    if (auto it = _srvs.find(name); _srvs.end() != it)
        return it->second;

    return nullptr;
}

ComPtr<ID3D11DepthStencilView> DeviceManager::FindDepthStencilView(const std::string& name) const
{
    if (auto it = _dsvs.find(name); _dsvs.end() != it)
        return it->second;

    return nullptr;
}

ComPtr<ID2D1Bitmap1> DeviceManager::FindBitmap(const std::string& name) const
{
    if (auto it = _bitmaps.find(name); _bitmaps.end() != it)
        return it->second;

    return nullptr;
}

ComPtr<ID3D11Texture2D> DeviceManager::CreateTexture2D(
  const std::string& name, D3D11_TEXTURE2D_DESC* desc)
{
    ComPtr<ID3D11Texture2D> texFound = FindTexture2D(name);
    if (texFound)
        return texFound;

    if (nullptr == desc)
        return nullptr;

    ComPtr<ID3D11Texture2D> tex = nullptr;
    if (FAILED(_device->CreateTexture2D(desc, NULL, tex.GetAddressOf())))
        return nullptr;

    _textures[name] = tex;
    return _textures[name];
}

ComPtr<ID3D11RenderTargetView> DeviceManager::CreateRenderTargetView(
  const std::string& name, ComPtr<ID3D11Texture2D> texture, D3D11_RENDER_TARGET_VIEW_DESC* desc)
{
    ComPtr<ID3D11RenderTargetView> rtvFound = FindRenderTargetView(name);
    if (rtvFound)
        return rtvFound;

    if (nullptr == texture)
        texture = _backBuffer;

    ComPtr<ID3D11RenderTargetView> rtv = nullptr;
    if (FAILED(_device->CreateRenderTargetView(texture.Get(), desc, rtv.GetAddressOf())))
        return nullptr;

    _rtvs[name] = rtv;
    return _rtvs[name];
}

ComPtr<ID3D11RenderTargetView> DeviceManager::CreateRenderTargetView(
  const std::string& name, const std::string& textureName, D3D11_RENDER_TARGET_VIEW_DESC* desc)
{
    ComPtr<ID3D11Texture2D> texture = FindTexture2D(textureName);
    return CreateRenderTargetView(name, texture, desc);
}

ComPtr<ID3D11ShaderResourceView> DeviceManager::CreateShaderResourceView(
  const std::string& name, ComPtr<ID3D11Texture2D> texture, D3D11_SHADER_RESOURCE_VIEW_DESC* desc)
{
    ComPtr<ID3D11ShaderResourceView> srvFound = FindShaderResourceView(name);
    if (srvFound)
        return srvFound;

    if (nullptr == texture)
        return nullptr;

    ComPtr<ID3D11ShaderResourceView> srv = nullptr;
    if (FAILED(_device->CreateShaderResourceView(texture.Get(), desc, srv.GetAddressOf())))
        return nullptr;

    _srvs[name] = srv;
    return _srvs[name];
}

ComPtr<ID3D11ShaderResourceView> DeviceManager::CreateShaderResourceView(
  const std::string& name, const std::string& textureName, D3D11_SHADER_RESOURCE_VIEW_DESC* desc)
{
    ComPtr<ID3D11Texture2D> texture = FindTexture2D(textureName);
    return CreateShaderResourceView(name, texture, desc);
}

ComPtr<ID3D11DepthStencilView> DeviceManager::CreateDepthStencilView(
  const std::string& name, ComPtr<ID3D11Texture2D> texture, D3D11_DEPTH_STENCIL_VIEW_DESC* desc)
{
    ComPtr<ID3D11DepthStencilView> dsvFound = FindDepthStencilView(name);
    if (dsvFound)
        return dsvFound;

    if (nullptr == texture)
        return nullptr;

    ComPtr<ID3D11DepthStencilView> dsv = nullptr;
    if (FAILED(_device->CreateDepthStencilView(texture.Get(), desc, dsv.GetAddressOf())))
        return nullptr;

    _dsvs[name] = dsv;
    return _dsvs[name];
}

ComPtr<ID3D11DepthStencilView> DeviceManager::CreateDepthStencilView(
  const std::string& name, const std::string& textureName, D3D11_DEPTH_STENCIL_VIEW_DESC* desc)
{
    ComPtr<ID3D11Texture2D> texture = FindTexture2D(textureName);
    return CreateDepthStencilView(name, texture, desc);
}

ComPtr<ID2D1Bitmap1> DeviceManager::CreateBitmap(
  const std::string& name, ComPtr<IDXGISurface> surface)
{
    ComPtr<ID2D1Bitmap1> bitmapFound = FindBitmap(name);
    if (bitmapFound)
        return bitmapFound;

    if (nullptr == surface)
        _swapChain->GetBuffer(0, IID_PPV_ARGS(surface.GetAddressOf()));

    ComPtr<ID2D1Bitmap1> bitmap = nullptr;
    HRESULT              hResult
      = _context2D->CreateBitmapFromDxgiSurface(surface.Get(), nullptr, bitmap.GetAddressOf());

    if (FAILED(hResult))
        return nullptr;

    _context2D->SetTarget(bitmap.Get());

    _bitmaps[name] = bitmap;
    return _bitmaps[name];
}

void DeviceManager::SetTarget()
{
    _context->OMSetRenderTargets(1, _rtvDefault.GetAddressOf(), _dsvDefault.Get());
}

void DeviceManager::Render()
{
    _swapChain->Present(0, 0);
}

void DeviceManager::ClearBackBuffer(float clearColor[4])
{
    _context->ClearRenderTargetView(_rtvDefault.Get(), clearColor);
}

void DeviceManager::ClearDepthStencil(float depth, uint8 stencil)
{
    _context->ClearDepthStencilView(
      _dsvDefault.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
