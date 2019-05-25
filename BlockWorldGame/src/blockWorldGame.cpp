#include "blockWorldGame.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine.h"

namespace bwg
{
GLuint vao;
engine::Shader shader;
engine::Texture texture;
engine::Camera camera;

void BWG::loadContent()
{
    float vertices[] = {
        // positions            // texture coords
        1.0f, 1.0f, 0.0f,  1.0f / 16, 1.0f / 16,   // top right 
        1.0f, 0.0f, 0.0f, 1.0f / 16, 0.0f / 16,  // bottom right
        0.0f, 0.0f, 0.0f, 0.0f / 16, 0.0f / 16, // bottom left
        0.0f, 1.0f, 0.0f, 0.0f / 16, 1.0f / 16   // top left

    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader.loadShader("content/shader/vs.vs", "", "content/shader/fr.fs");
    texture.loadTexture("content/textures/terrain.png", GL_RGBA);
    texture.use();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    camera.setFOV(45);
    camera.setAspectRatio((float)window.getWindowWidth() / (float)window.getWindowHeight());
    camera.setNearPlane(0.1f);
    camera.setFarPlane(100.0f);
    camera.updateProjectionMatrix();

    camera.setCameraPosition(glm::vec3(0.0f, 0.0f, -2.0f));
    camera.setCameraForward(glm::vec3(0.0f, 0.0f, 1.0f));
    camera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
    camera.updateViewProjectionMatrix();
}

void BWG::unloadContent()
{
    shader.deleteShader();
    texture.deleteTexture();
    glDeleteBuffers(1, &vao); 
}

void BWG::update(double timeStep)
{
}

void BWG::draw()
{
    glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    texture.use();
    shader.use();
    shader.setMatrix4fv("viewProjection", camera.getViewProjectionMatrix());
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
} // namespace bwg
