#ifndef MODEL_H
#define MODEL_H

#include "SceneObject.h"
#include "TRenderDevice.h"
#include <memory>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;
class Shader;
class Texture;

class Model : public SceneObject {
public:
    Model();

    bool Initialize(const std::string& meshPath);
    void Render(ID3D11DeviceContext* context) override;

    void SetShader(std::shared_ptr<Shader> shader);

private:
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<std::shared_ptr<Mesh>> meshes;
    std::shared_ptr<Shader> shader;
};

#endif // MODEL_H
