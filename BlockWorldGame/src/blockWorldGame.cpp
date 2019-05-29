#include "blockWorldGame.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

#include "engine.h"
#include "chunk.h"

namespace bwg
{
void BWG::initialize()
{
    Game::initialize();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //glfwSetInputMode(window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowPos(window.getGLFWwindow(), 1000, 400);

    camera.setFOV(45);
    camera.setAspectRatio((float)window.getWindowWidth() / (float)window.getWindowHeight());
    camera.setNearPlane(0.1f);
    camera.setFarPlane(100.0f);
    camera.updateProjectionMatrix();

    camera.setCameraPosition(glm::vec3(20.0f, 30.0f, 20.0f));
    camera.setPitch(-45);
    camera.setYaw(-135);
    camera.calculateCameraFront();
    camera.calculateCameraFront();
    camera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
    camera.updateViewProjectionMatrix();
}

void BWG::loadContent()
{
    terrain.loadContent();
    terrain.generateTerrain();
}

void BWG::unloadContent()
{
    terrain.unloadContent();
}

void BWG::cameraMovement(const double deltaTime)
{
    glm::vec3 direction = glm::vec3(0);
    bool hasMovedForward = false;
    bool hasMovedRight = false;
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_UP) == GLFW_PRESS)
    {
        hasMovedForward = true;
        direction += camera.getCameraFront();
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        hasMovedForward = !hasMovedForward;
        direction -= camera.getCameraFront();
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        hasMovedRight = true;
        direction += camera.getCameraRight();
    }
    if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        hasMovedRight = !hasMovedRight;
        direction -= camera.getCameraRight();
    }

    if (hasMovedForward || hasMovedRight)
        camera.move(glm::normalize(direction), cameraMovementSpeed * deltaTime);

    double xpos, ypos;
    glfwGetCursorPos(window.getGLFWwindow(), &xpos, &ypos);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= cameraRotationSpeed;
    yoffset *= cameraRotationSpeed;

    camera.rotate(xoffset, yoffset);
    camera.updateViewProjectionMatrix();
}

void BWG::update(const double deltaTime)
{
    cameraMovement(deltaTime);
}

void BWG::draw()
{
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    terrain.drawTerrain(camera);
}
} // namespace bwg
