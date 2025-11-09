#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Renderer.hpp"
#include <memory>

namespace lumen {

class Application
{
    public:
        Application();
        virtual ~Application();

        void run();

    private:
        void _init_window();
        void _main_loop();
        void _cleanup();

        static void _framebuffer_resize_callback(GLFWwindow *window, int width, int height);

        GLFWwindow *_window = nullptr;
        std::unique_ptr<Renderer> _renderer;
};

}// namespace lumen
