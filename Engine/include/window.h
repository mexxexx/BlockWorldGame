#ifndef __ENGINE_WINDOW_H__
#define __ENGINE_WINDOW_H__

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine
{
    class Window {
        public:
            Window() {}
            ~Window();
            bool isResizable();
            void setResizable(const bool resizable);
            std::string getTitle();
            void setTitle(const std::string title);
            int getWindowWidth();
            int getWindowHeight();
            /*
                Set the dimensions of the window. Passing negative values will leave the properties unchanged
            */
            void setWindowDimensions(const int width, const int height);
            GLFWwindow *getGLFWwindow();
            int initialize(const int width, const int height);
            bool shouldClose();
        private:
            std::string title;
            int window_width;
            int window_height;
            int resizable = GLFW_FALSE;

            GLFWwindow *glfwWindow;
    };
} // namespace engine

#endif