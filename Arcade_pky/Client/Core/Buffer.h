#pragma once

struct VertexBufferData
{
    std::vector<byte>    data;
    ComPtr<ID3D11Buffer> buffer = nullptr;
    uint32               size   = 0;
    int32                count  = 0;
};

struct IndexBufferData
{
    std::vector<byte>    data;
    ComPtr<ID3D11Buffer> buffer = nullptr;
    uint32               size   = 0;
    int32                count  = 0;
    DXGI_FORMAT          format = DXGI_FORMAT_UNKNOWN;
};
