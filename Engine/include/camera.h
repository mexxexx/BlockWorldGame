#ifndef __ENGINE_CAMERA_H__
#define __ENGINE_CAMERA_H__

#include "glm/glm.hpp"

namespace engine
{
    class Camera {
        public:
            void setCameraPosition(const glm::vec3 newPosition);
            glm::vec3 getCameraPosition() const;
            void setCameraUp(const glm::vec3 up);
            glm::vec3 getCameraUp() const;
            void setCameraForward(const glm::vec3 forward);
            glm::vec3 getCameraForward() const;
            glm::mat4 getViewProjectionMatrix() const;
            void updateViewProjectionMatrix();
            void updateProjectionMatrix();
            void setFOV(const float fov);
            float getFOV() const;
            void setAspectRatio(const float aspectRatio);
            float getAspectRatio() const;
            void setNearPlane(const float nearPlane);
            float getNearPlane() const;
            void setFarPlane(const float farPlane);
            float getFarPlane() const;
        private:
            glm::vec3 cameraPosition;
            glm::vec3 cameraUp;
            glm::vec3 cameraForward;
            glm::mat4 view;
            glm::mat4 viewProjection;
            glm::mat4 projection;
            float fov; //in degrees
            float aspectRatio;
            float nearPlane;
            float farPlane;
    };
} // namespace engine
#endif