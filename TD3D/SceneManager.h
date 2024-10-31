#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <vector>

#include "SceneObject.h"
#include "Camera.h"

class SceneManager {
public:
    SceneManager();

    void SetActiveCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> GetActiveCamera() const;

    void AddObject(std::shared_ptr<SceneObject> object);
    void Update();
    void Render(ID3D11DeviceContext* context);

private:
    std::vector<std::shared_ptr<SceneObject>> rootObjects;
    std::shared_ptr<Camera> activeCamera;
};

#endif // SCENEMANAGER_H
