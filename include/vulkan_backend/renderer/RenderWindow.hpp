#pragma once

#include <cstdint>
#include <functional>

struct GLFWwindow;

namespace lumen {

class Renderer;

class RenderWindow
{
    public:
        RenderWindow(const char *title, uint32_t width, uint32_t height);
        ~RenderWindow();

        RenderWindow(const RenderWindow &) = delete;
        RenderWindow &operator=(const RenderWindow &) = delete;

        bool should_close() const;
        void poll_events() const;

        GLFWwindow *get_native_handle() const;

        using FramebufferResizeCallback = std::function<void(int, int)>;
        void set_framebuffer_resize_callback(const FramebufferResizeCallback &callback);

    private:
        static void _glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height);

        GLFWwindow *_window = nullptr;
        FramebufferResizeCallback _resizeCallback;
};

}// namespace lumen
