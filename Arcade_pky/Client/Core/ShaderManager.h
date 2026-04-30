#pragma once
#include "SubManager.h"

#include "ConstantBuffer.h"
#include "Rendering.h"
#include "Shader.h"
// #include "SBuffer.h"

class ShaderManager : public SubManager
{
public:
    ShaderManager()           = default;
    ~ShaderManager() override = default;
    DELETE_SPECIAL_FUNC(ShaderManager)

private:
    std::unordered_map<std::string, Ptr<Shader>>         _shaders;
    std::unordered_map<std::string, Ptr<ConstantBuffer>> _constantBuffers;
    // std::unordered_map<std::string, Ptr<SBuffer>> _sBuffers;

    ComPtr<ID3D11SamplerState> _samplers[SamplerType::End];

public:
    bool Init() override;
    void Destroy() override;

    void SetSampler(uint8 type);

private:
    void CreateSamplers();

public:
    template <typename T>
    Ptr<T> FindShader(const std::string& name)
    {
        if (auto it = _shaders.find(name); _shaders.end() != it)
            return Cast<Shader, T>(it->second);

        return nullptr;
    }

    template <typename T>
    Ptr<T> FindConstantBuffer(const std::string& name)
    {
        if (auto it = _constantBuffers.find(name); _constantBuffers.end() != it)
            return Cast<ConstantBuffer, T>(it->second);

        return nullptr;
    }

    // template <typename T>
    // Ptr<T> FindSBuffer(const std::string& name)
    //{
    //     auto it = _sBuffers.find(name);
    //     if (_sBuffers.end() == it)
    //         return nullptr;

    //    return Cast<SBuffer, T>(it->second);
    //}

private:
    template <typename T>
    Ptr<T> CreateShader(const std::string& name)
    {
        Ptr<T> shaderFound = FindShader<T>(name);
        if (shaderFound)
            return shaderFound;

        Ptr<T> shader = New<T>();
        if (!shader->Init())
        {
            DESTROY(shader);
            return nullptr;
        }

        _shaders[name] = shader;
        return shader;
    }

    template <typename T>
    Ptr<T> CreateCBuffer(const std::string& name,
      uint32                                size,
      uint32                                registerNum,
      uint8                                 shaderType)
    {
        Ptr<T> constantBufferFound = FindConstantBuffer<T>(name);
        if (constantBufferFound)
            return constantBufferFound;

        Ptr<T> constantBuffer = New<T>();
        if (!constantBuffer->Create(size, registerNum, shaderType))
        {
            DESTROY(constantBuffer);
            return nullptr;
        }

        _constantBuffers[name] = constantBuffer;
        return constantBuffer;
    }

    // template <typename T>
    // bool CreateSBuffer(const std::string& name,
    //   int32                               size,
    //   int32                               elementCount,
    //   int32                               registerNum,
    //   int32                               shaderType)
    //{
    //     Ptr<T> structBufferFound = FindConstantBuffer<T>(name);
    //     if (structBufferFound)
    //         return structBufferFound;

    //    Ptr<T> structBuffer = New<T>();
    //    if (!structBuffer->Create(size, registerNum, shaderType))
    //    {
    //        DESTROY(structBuffer);
    //        return nullptr;
    //    }

    //    _sBuffers[name] = structBuffer;
    //    return _sBuffers[name];
    //}
};
