#include "SceneManager.h"
#include "SceneObject.h"
#include "Camera.h"

SceneManager::SceneManager() : activeCamera(nullptr) {}

void SceneManager::SetActiveCamera(std::shared_ptr<Camera> camera) {
    activeCamera = camera;
}

std::shared_ptr<Camera> SceneManager::GetActiveCamera() const {
    return activeCamera;
}

void SceneManager::AddObject(std::shared_ptr<SceneObject> object) {
    rootObjects.push_back(object);
}

void SceneManager::Update() {
    for (auto& object : rootObjects) {
        object->Update();
    }
}

void SceneManager::Render(ID3D11DeviceContext* context) {
    for (auto& object : rootObjects) {
        object->Render(context);
    }
}
