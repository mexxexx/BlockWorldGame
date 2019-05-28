#include "blockWorldGame.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

#include "engine.h"
#include "chunk.h"

namespace bwg
{
GLuint vao;
engine::Shader shader;
engine::Texture texture;
int numVerticies;

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
    Chunk c;
    Chunk::loadBlockTypes(); 
    c.generateChunk();
    std::vector<GLfloat> vertices;
    std::vector<GLint> indices;
    int vertexOffset = 0;
    c.getVisibleFaces(vertices, indices, vertexOffset);
    numVerticies = indices.size();

    GLuint vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    int stride = 5 * sizeof(GLfloat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader.loadShader("content/shader/terrain.vs", "", "content/shader/terrain.fs");

    texture.loadTexture("content/textures/terrain.png", GL_RGBA, false);
    texture.use();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void BWG::unloadContent()
{
    shader.deleteShader();
    texture.deleteTexture();
    glDeleteBuffers(1, &vao);
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

    texture.use();
    shader.use();
    shader.setMatrix4fv("viewProjection", camera.getViewProjectionMatrix());
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numVerticies, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
} // namespace bwg
