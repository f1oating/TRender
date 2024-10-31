#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <unordered_map>
#include <memory>

class Shader;

class ShaderManager {
public:
    ShaderManager();

    std::shared_ptr<Shader> LoadShader(const std::string& shaderName, const std::string& filePath,
        const std::string& entryPoint, const std::string& profile);

    std::shared_ptr<Shader> GetShader(const std::string& shaderName) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
};

#endif // SHADERMANAGER_H
