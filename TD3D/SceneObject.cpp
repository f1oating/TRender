#include "SceneObject.h"

SceneObject::SceneObject()
    : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), parent(nullptr) {}

void SceneObject::SetPosition(float x, float y, float z) {
    position = { x, y, z };
}

void SceneObject::SetRotation(float pitch, float yaw, float roll) {
    rotation = { pitch, yaw, roll };
}

void SceneObject::SetScale(float x, float y, float z) {
    scale = { x, y, z };
}

void SceneObject::AddChild(std::shared_ptr<SceneObject> child) {
    child->parent = this;
    children.push_back(child);
}

const std::vector<std::shared_ptr<SceneObject>>& SceneObject::GetChildren() const {
    return children;
}

DirectX::XMMATRIX SceneObject::GetWorldMatrix() const {
    using namespace DirectX;
    XMMATRIX translation = XMMatrixTranslation(position.x, position.y, position.z);
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    XMMATRIX scaling = XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX worldMatrix = scaling * rotationMatrix * translation;

    if (parent) {
        worldMatrix *= parent->GetWorldMatrix();
    }

    return worldMatrix;
}

void SceneObject::Update() {
    for (const auto& child : children) {
        child->Update();
    }
}

void SceneObject::Render(ID3D11DeviceContext* context) {
    for (const auto& child : children) {
        child->Render(context);
    }
}
