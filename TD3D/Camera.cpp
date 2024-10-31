#include "Camera.h"
#include <DirectXMath.h>

Camera::Camera()
    : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f) {}

void Camera::SetPosition(float x, float y, float z) {
    position = DirectX::XMFLOAT3(x, y, z);
}

void Camera::SetRotation(float pitch, float yaw, float roll) {
    rotation = DirectX::XMFLOAT3(pitch, yaw, roll);
}

DirectX::XMMATRIX Camera::GetViewMatrix() const {
    using namespace DirectX;

    float pitch = XMConvertToRadians(rotation.x);
    float yaw = XMConvertToRadians(rotation.y);
    float roll = XMConvertToRadians(rotation.z);

    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    XMVECTOR lookAt = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
    XMVECTOR up = XMVector3TransformCoord(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationMatrix);

    XMVECTOR positionVec = XMLoadFloat3(&position);
    return XMMatrixLookAtLH(positionVec, positionVec + lookAt, up);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix(float aspectRatio, float nearPlane, float farPlane) const {
    using namespace DirectX;
    return XMMatrixPerspectiveFovLH(XM_PIDIV4, aspectRatio, nearPlane, farPlane);
}
