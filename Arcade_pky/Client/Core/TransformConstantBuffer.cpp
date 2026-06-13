#include "pch.h"

#include "TransformConstantBuffer.h"

void TransformConstantBuffer::Update()
{
    _data.wv  = _data.world * _data.view;
    _data.wvp = _data.wv * _data.proj;

    _data.world.Transpose();
    _data.view.Transpose();
    _data.proj.Transpose();
    _data.wv.Transpose();
    _data.wvp.Transpose();

    BindData(&_data);
}
