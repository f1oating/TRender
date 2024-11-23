#include "TestCamera.h"

TestCamera::TestCamera()
    : pos(0.0, 0.0, 0.0), rot(0.0, 0.0, 0.0),
    viewMatrix(Eigen::Matrix4d::Identity()),
    projectionMatrix(Eigen::Matrix4d::Identity()),
    vec_forward(DEFAULT_FORWARD_VECTOR),
    vec_left(DEFAULT_LEFT_VECTOR),
    vec_right(DEFAULT_RIGHT_VECTOR),
    vec_backward(DEFAULT_BACKWARD_VECTOR)
{
    UpdateViewMatrix();
}

void TestCamera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    double fovRadians = (fovDegrees / 360.0) * 2 * PI;
    double tanHalfFov = tan(fovRadians / 2.0);

    projectionMatrix.setZero();
    projectionMatrix(0, 0) = 1.0 / (aspectRatio * tanHalfFov);
    projectionMatrix(1, 1) = 1.0 / tanHalfFov;
    projectionMatrix(2, 2) = farZ / (farZ - nearZ);
    projectionMatrix(3, 2) = -nearZ * farZ / (farZ - nearZ);
    projectionMatrix(2, 3) = 1.0;
}

const Eigen::Matrix4d& TestCamera::GetViewMatrix() const
{
    return viewMatrix;
}

const Eigen::Matrix4d& TestCamera::GetProjectionMatrix() const
{
    return projectionMatrix;
}

const Eigen::Vector3d& TestCamera::GetPosition() const
{
    return pos;
}

const Eigen::Vector3d& TestCamera::GetRotation() const
{
    return rot;
}

void TestCamera::SetPosition(const Eigen::Vector3d& pos)
{
    this->pos = pos;
    UpdateViewMatrix();
}

void TestCamera::SetPosition(double x, double y, double z)
{
    pos = { x, y, z };
    UpdateViewMatrix();
}

void TestCamera::AdjustPosition(const Eigen::Vector3d& offset)
{
    pos += offset;
    UpdateViewMatrix();
}

void TestCamera::AdjustPosition(double x, double y, double z)
{
    pos += Eigen::Vector3d{ x, y, z };
    UpdateViewMatrix();
}

void TestCamera::MoveForward(double distance)
{
    pos += vec_forward * distance;
    UpdateViewMatrix();
}

void TestCamera::MoveBackward(double distance)
{
    pos += vec_backward * distance;
    UpdateViewMatrix();
}

void TestCamera::MoveRight(double distance)
{
    pos += vec_right * distance;
    UpdateViewMatrix();
}

void TestCamera::MoveLeft(double distance)
{
    pos += vec_left * distance;
    UpdateViewMatrix();
}

void TestCamera::SetRotation(const Eigen::Vector3d& rot)
{
    this->rot = rot;
    UpdateViewMatrix();
}

void TestCamera::SetRotation(double x, double y, double z)
{
    rot = { x, y, z };
    UpdateViewMatrix();
}

void TestCamera::AdjustRotation(const Eigen::Vector3d& offset)
{
    rot += offset;
    UpdateViewMatrix();
}

void TestCamera::AdjustRotation(double x, double y, double z)
{
    rot += Eigen::Vector3d{ x, y, z };
    UpdateViewMatrix();
}

void TestCamera::SetLookAtPos(const Eigen::Vector3d& lookAtPos)
{
    if (pos == lookAtPos)
        return;

    Eigen::Vector3d direction = lookAtPos - pos;
    direction.normalize();

    double pitch = atan2(direction.y(), sqrt(direction.x() * direction.x() + direction.z() * direction.z()));
    double yaw = atan2(direction.x(), direction.z());
    SetRotation(pitch, yaw, 0.0);
}

const Eigen::Vector3d& TestCamera::GetForwardVector()
{
    return vec_forward;
}

const Eigen::Vector3d& TestCamera::GetRightVector()
{
    return vec_right;
}

const Eigen::Vector3d& TestCamera::GetBackwardVector()
{
    return vec_backward;
}

const Eigen::Vector3d& TestCamera::GetLeftVector()
{
    return vec_left;
}

void TestCamera::UpdateViewMatrix()
{
    Eigen::Matrix3d rotationMatrix;
    rotationMatrix = Eigen::AngleAxisd(rot.y(), Eigen::Vector3d::UnitY()) *
        Eigen::AngleAxisd(rot.x(), Eigen::Vector3d::UnitX()) *
        Eigen::AngleAxisd(rot.z(), Eigen::Vector3d::UnitZ());

    Eigen::Vector3d forward = rotationMatrix * DEFAULT_FORWARD_VECTOR;
    Eigen::Vector3d up = rotationMatrix * DEFAULT_UP_VECTOR;

    vec_forward = forward;
    vec_backward = -forward;
    vec_left = rotationMatrix * DEFAULT_LEFT_VECTOR;
    vec_right = rotationMatrix * DEFAULT_RIGHT_VECTOR;

    Eigen::Vector3d target = pos + forward;

    Eigen::Matrix4d view;
    view.setIdentity();
    view.block<3, 3>(0, 0) = rotationMatrix.transpose();
    view.block<3, 1>(0, 3) = -rotationMatrix.transpose() * pos;

    viewMatrix = view;
}
