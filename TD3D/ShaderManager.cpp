#include "ShaderManager.h"
#include "TRenderDevice.h"
#include "Shader.h"
#include <stdexcept>

ShaderManager ShaderManager::m_ShaderManager;

ShaderManager::ShaderManager() {}

std::shared_ptr<Shader> ShaderManager::LoadShader(const std::string& shaderName, const std::string& filePath,
    const std::string& entryPoint, const std::string& profile) {

    if (shaders.find(shaderName) != shaders.end()) {
        throw std::runtime_error("Shader already loaded: " + shaderName);
    }

    std::shared_ptr<Shader> newShader = std::make_shared<Shader>();

    ID3D11Device* device = TRenderDevice::GetRenderDevice()->GetDevice();

    if (!newShader->LoadFromFile(device, filePath, profile)) {
        throw std::runtime_error("Failed to load shader: " + shaderName);
    }

    shaders[shaderName] = newShader;

    return newShader;
}

std::shared_ptr<Shader> ShaderManager::GetShader(const std::string& shaderName) const {
    auto it = shaders.find(shaderName);
    if (it != shaders.end()) {
        return it->second;
    }
    throw std::runtime_error("Shader not found: " + shaderName);
}

ShaderManager* ShaderManager::GetShaderManager()
{
    return &m_ShaderManager;
}

ShaderManager* GetShaderManager()
{
    return ShaderManager::GetShaderManager();
}
