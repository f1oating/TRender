#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include <vector>

#include "SceneObject.h"
#include "Camera.h"

class SceneManager {
protected:
    SceneManager();
    static SceneManager m_SceneManager;

public:
    void SetActiveCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> GetActiveCamera() const;

    void AddObject(std::shared_ptr<SceneObject> object);
    void Update();
    void Render(ID3D11DeviceContext* context);

    static SceneManager* GetSceneManager();

private:
    std::vector<std::shared_ptr<SceneObject>> rootObjects;
    std::shared_ptr<Camera> activeCamera;
};

extern "C"
{
    SceneManager* GetSceneManager();
    typedef SceneManager* (*GetSceneManagerFunc)();
}

#endif // SCENEMANAGER_H
