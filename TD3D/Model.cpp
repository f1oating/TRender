#include "Model.h"
#include "TRenderDevice.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

Model::Model() : mesh(nullptr), shader(nullptr), texture(nullptr) {}

bool Model::Initialize(TRenderDevice* renderDevice, const std::string& meshPath, const std::string& texturePath) {
    mesh = std::make_shared<Mesh>();
    if (!mesh->LoadFromFile(meshPath)) {
        return false;
    }

    texture = std::make_shared<Texture>();
    if (!texture->LoadFromFile(texturePath, renderDevice->GetDevice())) {
        return false;
    }

    return true;
}

void Model::Render(ID3D11DeviceContext* context) {
    if (shader) {
        shader->Bind(context);
    }
    if (texture) {
        texture->Bind(context, 0);
    }
    if (mesh) {
        // Bind world matrix before rendering
        mesh->Render(context);
    }

    // Rendering child objects
    SceneObject::Render(context);
}

void Model::SetShader(std::shared_ptr<Shader> newShader) {
    shader = newShader;
}

void Model::SetTexture(std::shared_ptr<Texture> newTexture) {
    texture = newTexture;
}
