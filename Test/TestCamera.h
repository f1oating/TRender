#pragma once
#include "Eigen\Dense"
#include <cmath>

const double PI = 3.14159265358979323846;

class TestCamera
{
public:
    TestCamera();

    void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

    const Eigen::Matrix4d& GetViewMatrix() const;
    const Eigen::Matrix4d& GetProjectionMatrix() const;

    const Eigen::Vector3d& GetPosition() const;
    const Eigen::Vector3d& GetRotation() const;

    void SetPosition(const Eigen::Vector3d& pos);
    void SetPosition(double x, double y, double z);
    void AdjustPosition(const Eigen::Vector3d& pos);
    void AdjustPosition(double x, double y, double z);

    void SetRotation(const Eigen::Vector3d& rot);
    void SetRotation(double pitch, double yaw, double roll);
    void AdjustRotation(const Eigen::Vector3d& rot);
    void AdjustRotation(double pitch, double yaw, double roll);

    void SetLookAtPos(const Eigen::Vector3d& lookAtPos);

    const Eigen::Vector3d& GetForwardVector();
    const Eigen::Vector3d& GetRightVector();
    const Eigen::Vector3d& GetBackwardVector();
    const Eigen::Vector3d& GetLeftVector();

private:
    void UpdateViewMatrix();

    Eigen::Vector3d pos;
    Eigen::Vector3d rot;

    Eigen::Matrix4d viewMatrix;
    Eigen::Matrix4d projectionMatrix;

    const Eigen::Vector3d DEFAULT_FORWARD_VECTOR = { 0.0, 0.0, 1.0 };
    const Eigen::Vector3d DEFAULT_UP_VECTOR = { 0.0, 1.0, 0.0 };
    const Eigen::Vector3d DEFAULT_BACKWARD_VECTOR = { 0.0, 0.0, -1.0 };
    const Eigen::Vector3d DEFAULT_LEFT_VECTOR = { -1.0, 0.0, 0.0 };
    const Eigen::Vector3d DEFAULT_RIGHT_VECTOR = { 1.0, 0.0, 0.0 };

    Eigen::Vector3d vec_forward;
    Eigen::Vector3d vec_left;
    Eigen::Vector3d vec_right;
    Eigen::Vector3d vec_backward;
};
