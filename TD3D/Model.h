#ifndef MODEL_H
#define MODEL_H

#include "SceneObject.h"
#include <memory>

class TRenderDevice;
class Mesh;
class Shader;
class Texture;

class Model : public SceneObject {
public:
    Model();

    bool Initialize(TRenderDevice* renderDevice, const std::string& meshPath, const std::string& texturePath);
    void Render(ID3D11DeviceContext* context) override;

    void SetShader(std::shared_ptr<Shader> shader);
    void SetTexture(std::shared_ptr<Texture> texture);

private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
};

#endif // MODEL_H
