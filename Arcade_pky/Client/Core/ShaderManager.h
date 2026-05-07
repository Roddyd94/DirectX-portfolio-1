#pragma once
#include "SubManager.h"

#include "ConstantBuffer.h"
#include "Rendering.h"
#include "Shader.h"
#include "StructureBuffer.h"

class ShaderManager : public SubManager
{
public:
    ShaderManager()           = default;
    ~ShaderManager() override = default;
    DELETE_SPECIAL_FUNC(ShaderManager)

private:
    std::unordered_map<std::string, int32> _shaderFinder;
    std::unordered_map<std::string, int32> _constantBufferFinder;
    std::unordered_map<std::string, int32> _structureBufferFinder;

    std::map<int32, Ptr<Shader>>          _shaders;
    std::map<int32, Ptr<ConstantBuffer>>  _constantBuffers;
    std::map<int32, Ptr<StructureBuffer>> _structureBuffers;

    int32 _shaderIDCounter          = 0;
    int32 _constantBufferIDCounter  = 0;
    int32 _structureBufferIDCounter = 0;

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
        auto itFinder = _shaderFinder.find(name);
        if (_shaderFinder.end() == itFinder)
            return nullptr;

        int32 id       = itFinder->second;
        auto  itShader = _shaders.find(id);
        if (_shaders.end() == itShader)
            return nullptr;

        return Cast<Shader, T>(itShader->second);
    }

    template <typename T>
    Ptr<T> FindConstantBuffer(const std::string& name)
    {
        auto itFinder = _constantBufferFinder.find(name);
        if (_constantBufferFinder.end() == itFinder)
            return nullptr;

        int32 id       = itFinder->second;
        auto  itBuffer = _constantBuffers.find(id);
        if (_constantBuffers.end() == itBuffer)
            return nullptr;

        return Cast<ConstantBuffer, T>(itBuffer->second);
    }

    template <typename T>
    Ptr<T> FindStructureBuffer(const std::string& name)
    {
        auto itFinder = _structureBufferFinder.find(name);
        if (_structureBufferFinder.end() == itFinder)
            return nullptr;

        int32 id       = itFinder->second;
        auto  itBuffer = _structureBuffers.find(id);
        if (_structureBuffers.end() == itBuffer)
            return nullptr;

        return Cast<StructureBuffer, T>(itBuffer->second);
    }

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

        Ptr<Resource> resource = shader;
        resource->SetName(name);
        resource->SetID(_shaderIDCounter);
        _shaders[_shaderIDCounter] = shader;
        _shaderFinder[name]        = _shaderIDCounter;
        _shaderIDCounter++;

        return shader;
    }

    template <typename T>
    Ptr<T> CreateConstantBuffer(
      const std::string& name, uint32 size, int32 registerNum, uint8 shaderType)
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

        Ptr<Resource> resource = constantBuffer;
        resource->SetName(name);
        resource->SetID(_constantBufferIDCounter);
        _constantBuffers[_constantBufferIDCounter] = constantBuffer;
        _constantBufferFinder[name]                = _constantBufferIDCounter;
        _constantBufferIDCounter++;

        return constantBuffer;
    }

    template <typename T>
    Ptr<T> CreateStructureBuffer(
      const std::string& name, int32 size, int32 elementCount, int32 registerNum, int32 shaderType)
    {
        Ptr<T> structureBufferFound = FindConstantBuffer<T>(name);
        if (structureBufferFound)
            return structureBufferFound;

        Ptr<T> structureBuffer = New<T>();
        if (!structureBuffer->Create(size, elementCount, registerNum, shaderType))
        {
            DESTROY(structureBuffer);
            return nullptr;
        }

        Ptr<Resource> resource = structureBuffer;
        resource->SetName(name);
        resource->SetID(_structureBufferIDCounter);
        _structureBuffers[_structureBufferIDCounter] = structureBuffer;
        _structureBufferFinder[name]                 = _structureBufferIDCounter;
        _structureBufferIDCounter++;

        return structureBuffer;
    }
};
