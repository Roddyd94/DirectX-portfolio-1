#include "pch.h"

#include "Material.h"

#include "ResourceManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include "MaterialConstantBuffer.h"
#include "Shader.h"
#include "Texture.h"

void Material::Destroy() {}

void Material::SetBaseColor(float r, float g, float b, float a)
{
    _baseColor = Color{r, g, b, a};
}

void Material::SetBaseColor(Color color)
{
    _baseColor = color;
}

void Material::SetOpacity(float opacity)
{
    _baseColor.SetA(opacity);
}

void Material::SetSamplerType(uint8 type)
{
    _samplerType = type;
}

void Material::AddTexture(Ptr<Texture> texture,
  uint32                               registerNum,
  uint32                               shaderBufferType,
  uint32                               textureIndex)
{
    MaterialTextureInfo info;
    info.texture          = texture;
    info.registerNum      = registerNum;
    info.shaderBufferType = shaderBufferType;
    info.textureIndex     = textureIndex;

    auto cBuffer = Lock(_constantBuffer);
    cBuffer->SetTextureWidth(texture->GetTexture(textureIndex)->width);
    cBuffer->SetTextureHeight(texture->GetTexture(textureIndex)->height);

    _textureInfos.emplace_back(info);
}

void Material::AddTexture(const std::string& name,
  uint32                                     registerNum,
  uint32                                     shaderBufferType,
  uint32                                     textureIndex)
{
    Ptr<Texture> texture = TEXTURE_MANAGER->FindTexture(name);
    if (nullptr == texture)
        return;

    AddTexture(texture, registerNum, shaderBufferType, textureIndex);
}

void Material::SetPixelShader(const std::string& name)
{
    _pixelShader = SHADER_MANAGER->FindShader<Shader>(name);
}

void Material::SetMaterial()
{
    auto cBuffer = Lock(_constantBuffer);
    cBuffer->SetBaseColor(_baseColor);
    cBuffer->Update();

    SHADER_MANAGER->SetSampler(_samplerType);

    auto ps = Lock(_pixelShader);
    ps->SetShader();

    for (auto& textureInfo : _textureInfos)
    {
        Ptr<Texture> texture = Lock<Texture>(textureInfo.texture);
        if (nullptr == texture)
            continue;

        texture->SetShader(textureInfo.registerNum,
          textureInfo.shaderBufferType, textureInfo.textureIndex);
    }
}

void Material::ResetMaterial()
{
    for (auto& textureInfo : _textureInfos)
    {
        Ptr<Texture> texture = Lock<Texture>(textureInfo.texture);
        if (nullptr == texture)
            continue;

        texture->ResetShader(
          textureInfo.registerNum, textureInfo.shaderBufferType);
    }
}

Ptr<Material> Material::Clone()
{
    Ptr<Material> clone = New<Material>();

    clone->_baseColor      = _baseColor;
    clone->_samplerType    = _samplerType;
    clone->_pixelShader    = _pixelShader;
    clone->_constantBuffer = _constantBuffer;
    clone->_textureInfos   = _textureInfos;

    return clone;
}
