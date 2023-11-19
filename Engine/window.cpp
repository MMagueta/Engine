#include "window.hpp"
#include "vulkan/vulkan_core.h"
#include <stdexcept>

namespace engine {
    
    Window::Window(int width, int height, std::string windowName): width{width}, height{height}, windowName{windowName}{
        initWindow();
    }

    Window::~Window(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::initWindow(){
        glfwInit();
        // No OpenGL, Vulkan instead
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
    
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface){
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create window surface");
    }

    void Window::frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
        Window* callbackWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        callbackWindow->frameBufferResized = true;
        callbackWindow->width = width;
        callbackWindow->height = height;
        return;
    }
}