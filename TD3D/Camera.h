#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

class Camera {
public:
    Camera();

    void SetPosition(float x, float y, float z);
    void SetRotation(float pitch, float yaw, float roll);

    DirectX::XMMATRIX GetViewMatrix() const;
    DirectX::XMMATRIX GetProjectionMatrix(float aspectRatio, float nearPlane = 0.1f, float farPlane = 1000.0f) const;

private:
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
};

#endif // CAMERA_H
