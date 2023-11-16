#pragma once

#define GLFW_INCLUDE_VULKAN

//#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <string>

namespace engine {
    class Window{
        public:
        Window(int width, int height, std::string windowName);
        ~Window(); // Destructor

        Window(const Window&) = delete;
        Window &operator=(const Window&) = delete;

        bool shouldClose() {return glfwWindowShouldClose(window);}

        void createWindowSurface(VkInstance, VkSurfaceKHR*);

        private:
        
        void initWindow();
        const int width;
        const int height;

        std::string windowName;

        GLFWwindow *window;
    };
}