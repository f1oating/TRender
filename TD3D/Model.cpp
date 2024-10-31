#include "Model.h"
#include "TRenderDevice.h"
#include "Mesh.h"
#include "Shader.h"

Model::Model() : shader(nullptr) {}

bool Model::Initialize(const std::string& meshPath) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(meshPath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return false; // Handle loading error
    }

    // Clear existing meshes
    meshes.clear();

    // Process all nodes
    ProcessNode(scene->mRootNode, scene);

    return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    // Process each mesh in the node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(std::make_shared<Mesh>(ProcessMesh(mesh, scene)));
    }

    // Process each child node recursively
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh result;

    // Process vertices
    std::vector<Vertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Positions
        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        // Normals
        vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

        // Texture coordinates
        if (mesh->mTextureCoords[0]) { // Check if texture coordinates exist
            vertex.texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }
        else {
            vertex.texcoord = { 0.0f, 0.0f };
        }

        vertices.push_back(vertex);
    }

    // Process indices
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    result.SetVertices(vertices);
    result.SetIndices(indices);
    result.InitializeBuffers(TRenderDevice::GetRenderDevice()->GetDevice());

    return result;
}

void Model::Render(ID3D11DeviceContext* context) {
    if (shader) {
        shader->Bind(context);
    }
    for (const auto& mesh : meshes) {
        mesh->Render(context);
    }

    // Rendering child objects
    SceneObject::Render(context);
}

void Model::SetShader(std::shared_ptr<Shader> newShader) {
    shader = newShader;
}
