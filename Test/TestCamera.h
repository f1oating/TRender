#pragma once
#include "Eigen\Dense"
#include <cmath>

const float PI = 3.14159265358979323846;

class TestCamera
{
public:
    TestCamera();

    void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

    const Eigen::Matrix4f& GetViewMatrix() const;
    const Eigen::Matrix4f& GetProjectionMatrix() const;

    const Eigen::Vector3f& GetPosition() const;
    const Eigen::Vector3f& GetRotation() const;

    void SetPosition(const Eigen::Vector3f& pos);
    void SetPosition(float x, float y, float z);
    void AdjustPosition(const Eigen::Vector3f& pos);
    void AdjustPosition(float x, float y, float z);

    void MoveForward(float distance);
    void MoveBackward(float distance);
    void MoveRight(float distance);
    void MoveLeft(float distance);

    void SetRotation(const Eigen::Vector3f& rot);
    void SetRotation(float pitch, float yaw, float roll);
    void AdjustRotation(const Eigen::Vector3f& rot);
    void AdjustRotation(float pitch, float yaw, float roll);

    void SetLookAtPos(const Eigen::Vector3f& lookAtPos);

    const Eigen::Vector3f& GetForwardVector();
    const Eigen::Vector3f& GetRightVector();
    const Eigen::Vector3f& GetBackwardVector();
    const Eigen::Vector3f& GetLeftVector();

private:
    void UpdateViewMatrix();

    Eigen::Vector3f pos;
    Eigen::Vector3f rot;

    Eigen::Matrix4f viewMatrix;
    Eigen::Matrix4f projectionMatrix;

    const Eigen::Vector3f DEFAULT_FORWARD_VECTOR = { 0.0, 0.0, 1.0 };
    const Eigen::Vector3f DEFAULT_UP_VECTOR = { 0.0, 1.0, 0.0 };
    const Eigen::Vector3f DEFAULT_BACKWARD_VECTOR = { 0.0, 0.0, -1.0 };
    const Eigen::Vector3f DEFAULT_LEFT_VECTOR = { -1.0, 0.0, 0.0 };
    const Eigen::Vector3f DEFAULT_RIGHT_VECTOR = { 1.0, 0.0, 0.0 };

    Eigen::Vector3f vec_forward;
    Eigen::Vector3f vec_left;
    Eigen::Vector3f vec_right;
    Eigen::Vector3f vec_backward;
};
