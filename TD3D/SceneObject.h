#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <DirectXMath.h>
#include <d3d11.h>
#include <memory>
#include <vector>

class SceneObject {
public:
    SceneObject();

    void SetPosition(float x, float y, float z);
    void SetRotation(float pitch, float yaw, float roll);
    void SetScale(float x, float y, float z);

    void AddChild(std::shared_ptr<SceneObject> child);
    const std::vector<std::shared_ptr<SceneObject>>& GetChildren() const;

    DirectX::XMMATRIX GetWorldMatrix() const;
    virtual void Update();
    virtual void Render(ID3D11DeviceContext* context);

protected:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scale;
    SceneObject* parent;
    std::vector<std::shared_ptr<SceneObject>> children;
};

#endif // SCENEOBJECT_H
