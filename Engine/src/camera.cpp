#include "camera.h"

#include "glm/ext.hpp"

namespace engine
{
void Camera::setCameraPosition(const glm::vec3 newPosition)
{
    this->cameraPosition = newPosition;
}

glm::vec3 Camera::getCameraPosition() const
{
    return this->cameraPosition;
}

void Camera::setCameraForward(const glm::vec3 forward)
{
    this->cameraForward = forward;
}

glm::vec3 Camera::getCameraForward() const
{
    return this->cameraForward;
}

void Camera::setCameraUp(const glm::vec3 up)
{
    this->cameraUp = up;
}

glm::vec3 Camera::getCameraUp() const
{
    return this->cameraUp;
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
    return this->viewProjection;
}

void Camera::updateViewProjectionMatrix()
{
    this->view = glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);
    this->viewProjection = projection * view;
}

void Camera::updateProjectionMatrix()
{
    this->projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::setFOV(const float fov)
{
    this->fov = fov;
}

float Camera::getFOV() const
{
    return this->fov;
}

void Camera::setAspectRatio(const float aspectRatio)
{
    this->aspectRatio = aspectRatio;
}

float Camera::getAspectRatio() const
{
    return this->aspectRatio;
}

void Camera::setNearPlane(const float nearPlane)
{
    this->nearPlane = nearPlane;
}

float Camera::getNearPlane() const
{
    return this->nearPlane;
}

void Camera::setFarPlane(const float farPlane)
{
    this->farPlane = farPlane;
}

float Camera::getFarPlane() const
{
    return this->farPlane;
}
} // namespace engine