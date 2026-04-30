#pragma once
#include "ConstantBuffer.h"

struct TransformConstantBufferData
{
    Matrix world;
    Matrix view;
    Matrix proj;
    Matrix wv;
    Matrix wvp;
};

class TransformConstantBuffer : public ConstantBuffer
{
public:
    TransformConstantBuffer()          = default;
    virtual ~TransformConstantBuffer() = default;

private:
    TransformConstantBufferData _data;

public:
    virtual void Update();

    void SetWorldMatrix(const Matrix& mat) { _data.world = mat; }
    void SetViewMatrix(const Matrix& mat) { _data.view = mat; }
    void SetProjMatrix(const Matrix& mat) { _data.proj = mat; }
};
