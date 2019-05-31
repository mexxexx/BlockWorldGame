#include "game.h"

#include <sstream>

#include <GLFW/glfw3.h>

namespace engine
{
void Game::run()
{
    this->shouldRun = true;
    initialize();
    if (this-> shouldRun) 
        loadContent();

    double lastTime = glfwGetTime();
    double nowTime = lastTime;
    double accumulatedTime = 0;
    double deltaTime = 0;
    int frames = 0;
    int updates = 0;

    double timer = 0;

    // - While window is alive
    while (this->shouldRun)
    {
        this->shouldRun = !window.shouldClose();

        // - Measure time
        lastTime = nowTime;
        nowTime = glfwGetTime();
        deltaTime = (nowTime - lastTime);
        accumulatedTime += deltaTime;
        timer += deltaTime;

        // - Update at X FPS
        while (accumulatedTime >= timeStep)
        {
            update(timeStep); // - Update function
            updates++;
            accumulatedTime-=timeStep;

        }
        // - Render at maximum possible frames
        draw(); // - Render function
        glfwSwapBuffers(window.getGLFWwindow());
        glfwPollEvents();

        frames++;

        // - Reset after one second
        if (timer > 1)
        {
            timer--;
            std::stringstream ss;
            ss << "FPS: " << frames << " - UPS: " << updates;
            window.setTitle(ss.str());
            updates = 0, frames = 0;
        }
    }
    unloadContent();
}

void Game::initialize()
{
    timeStep = 1.0/60;
    int windowResult = window.initialize(1280, 720);
    if (!windowResult) {
        this->shouldRun = false;
        return;
    }
}
} // namespace engine