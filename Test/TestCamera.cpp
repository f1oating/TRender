#include "TestCamera.h"

TestCamera::TestCamera()
    : pos(0.0, 0.0, 0.0), rot(0.0, 0.0, 0.0),
    viewMatrix(Eigen::Matrix4f::Identity()),
    projectionMatrix(Eigen::Matrix4f::Identity()),
    vec_forward(DEFAULT_FORWARD_VECTOR),
    vec_left(DEFAULT_LEFT_VECTOR),
    vec_right(DEFAULT_RIGHT_VECTOR),
    vec_backward(DEFAULT_BACKWARD_VECTOR)
{
    UpdateViewMatrix();
}

void TestCamera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
    float fovRadians = (fovDegrees / 360.0) * 2 * PI;
    float tanHalfFov = tan(fovRadians / 2.0);

    projectionMatrix.setZero();
    projectionMatrix(0, 0) = 1.0 / (aspectRatio * tanHalfFov);
    projectionMatrix(1, 1) = 1.0 / tanHalfFov;
    projectionMatrix(2, 2) = farZ / (farZ - nearZ);
    projectionMatrix(3, 2) = -nearZ * farZ / (farZ - nearZ);
    projectionMatrix(2, 3) = 1.0;
}

const Eigen::Matrix4f& TestCamera::GetViewMatrix() const
{
    return viewMatrix;
}

const Eigen::Matrix4f& TestCamera::GetProjectionMatrix() const
{
    return projectionMatrix;
}

const Eigen::Vector3f& TestCamera::GetPosition() const
{
    return pos;
}

const Eigen::Vector3f& TestCamera::GetRotation() const
{
    return rot;
}

void TestCamera::SetPosition(const Eigen::Vector3f& pos)
{
    this->pos = pos;
    UpdateViewMatrix();
}

void TestCamera::SetPosition(float x, float y, float z)
{
    pos = { x, y, z };
    UpdateViewMatrix();
}

void TestCamera::AdjustPosition(const Eigen::Vector3f& offset)
{
    pos += offset;
    UpdateViewMatrix();
}

void TestCamera::AdjustPosition(float x, float y, float z)
{
    pos += Eigen::Vector3f{ x, y, z };
    UpdateViewMatrix();
}

void TestCamera::MoveForward(float distance)
{
    pos += vec_forward * distance;
    UpdateViewMatrix();
}

void TestCamera::MoveBackward(float distance)
{
    pos += vec_backward * distance;
    UpdateViewMatrix();
}

void TestCamera::MoveRight(float distance)
{
    pos += vec_right * distance;
    UpdateViewMatrix();
}

void TestCamera::MoveLeft(float distance)
{
    pos += vec_left * distance;
    UpdateViewMatrix();
}

void TestCamera::SetRotation(const Eigen::Vector3f& rot)
{
    this->rot = rot;
    UpdateViewMatrix();
}

void TestCamera::SetRotation(float x, float y, float z)
{
    rot = { x, y, z };
    UpdateViewMatrix();
}

void TestCamera::AdjustRotation(const Eigen::Vector3f& offset)
{
    rot += offset;
    UpdateViewMatrix();
}

void TestCamera::AdjustRotation(float x, float y, float z)
{
    rot += Eigen::Vector3f{ x, y, z };
    UpdateViewMatrix();
}

void TestCamera::SetLookAtPos(const Eigen::Vector3f& lookAtPos)
{
    if (pos == lookAtPos)
        return;

    Eigen::Vector3f direction = lookAtPos - pos;
    direction.normalize();

    float pitch = atan2(direction.y(), sqrt(direction.x() * direction.x() + direction.z() * direction.z()));
    float yaw = atan2(direction.x(), direction.z());
    SetRotation(pitch, yaw, 0.0);
}

const Eigen::Vector3f& TestCamera::GetForwardVector()
{
    return vec_forward;
}

const Eigen::Vector3f& TestCamera::GetRightVector()
{
    return vec_right;
}

const Eigen::Vector3f& TestCamera::GetBackwardVector()
{
    return vec_backward;
}

const Eigen::Vector3f& TestCamera::GetLeftVector()
{
    return vec_left;
}

void TestCamera::UpdateViewMatrix()
{
    Eigen::Matrix3f rotationMatrix;
    rotationMatrix = Eigen::AngleAxisf(rot.y(), Eigen::Vector3f::UnitY()) *
        Eigen::AngleAxisf(rot.x(), Eigen::Vector3f::UnitX()) *
        Eigen::AngleAxisf(rot.z(), Eigen::Vector3f::UnitZ());

    Eigen::Vector3f forward = rotationMatrix * DEFAULT_FORWARD_VECTOR;
    Eigen::Vector3f up = rotationMatrix * DEFAULT_UP_VECTOR;

    vec_forward = forward;
    vec_backward = -forward;
    vec_left = rotationMatrix * DEFAULT_LEFT_VECTOR;
    vec_right = rotationMatrix * DEFAULT_RIGHT_VECTOR;

    Eigen::Vector3f target = pos + forward;

    Eigen::Matrix4f view;
    view.setIdentity();
    view.block<3, 3>(0, 0) = rotationMatrix.transpose();
    view.block<3, 1>(0, 3) = -rotationMatrix.transpose() * pos;

    viewMatrix = view;
}
