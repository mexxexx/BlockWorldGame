#include "camera.h"

#include "glm/ext.hpp"

namespace engine
{
void Camera::setCameraPosition(const glm::vec3 &newPosition)
{
    this->cameraPosition = newPosition;
}

glm::vec3 Camera::getCameraPosition() const
{
    return this->cameraPosition;
}

glm::vec3 Camera::getCameraFront() const
{
    return this->cameraFront;
}

void Camera::setCameraUp(const glm::vec3 &up)
{
    this->cameraUp = up;
    calculateCameraRight();
}

glm::vec3 Camera::getCameraUp() const
{
    return this->cameraUp;
}

glm::vec3 Camera::getCameraRight() const
{
    return this->cameraRight;
}

glm::mat4 Camera::getViewProjectionMatrix() const
{
    return this->viewProjection;
}

void Camera::updateViewProjectionMatrix()
{
    this->view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
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

void Camera::move(const glm::vec3 &direction, const float amount)
{
    this->cameraPosition += amount * direction;
}

void Camera::calculateCameraRight()
{
    this->cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
}

void Camera::setPitch(const float pitch)
{
    this->pitch = pitch;
    if(pitch > 89.0f)
        this->pitch = 89.0f;
    if(pitch < -89.0f)
        this->pitch = -89.0f;
}

float Camera::getPitch() const
{
    return this->pitch;
}

void Camera::setYaw(const float yaw)
{
    this->yaw = yaw;
}

float Camera::getYaw() const
{
    return this->yaw;
}

void Camera::calculateCameraFront()
{
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    this->cameraFront = front;
    calculateCameraRight();
}

void Camera::rotate(const double xoffset, const double yoffset) {
    setYaw(yaw + xoffset);
    setPitch(pitch + yoffset);
    calculateCameraFront();
}
} // namespace engine