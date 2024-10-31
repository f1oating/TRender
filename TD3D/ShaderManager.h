#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>

class Shader;

class ShaderManager {
    ShaderManager();
    static ShaderManager m_ShaderManager;

public:
    std::shared_ptr<Shader> LoadShader(const std::string& shaderName, const std::string& filePath,
        const std::string& entryPoint, const std::string& profile);

    std::shared_ptr<Shader> GetShader(const std::string& shaderName) const;

    static ShaderManager* GetShaderManager();

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

};

extern "C"
{
    ShaderManager* GetShaderManager();
    typedef ShaderManager* (*GetShaderManagerFunc)();
}

#endif // SHADERMANAGER_H
