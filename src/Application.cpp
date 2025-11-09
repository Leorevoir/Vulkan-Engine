#include "Application.hpp"
#include "vulkan_backend/utils/Config.hpp"

/**
* public
*/

lumen::Application::Application()
{
    _init_window();
    _renderer = std::make_unique<Renderer>(_window);
}

lumen::Application::~Application()
{
    _cleanup();
}

void lumen::Application::run()
{
    _main_loop();
}

/**
* private
*/

void lumen::Application::_init_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _window = glfwCreateWindow(config::WINDOW_WIDTH, config::WINDOW_HEIGHT, "Lumen Engine", nullptr, nullptr);

    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, _framebuffer_resize_callback);
}

void lumen::Application::_main_loop()
{
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        _renderer->draw_frame();
    }
}

void lumen::Application::_cleanup()
{
    _renderer.reset();

    glfwDestroyWindow(_window);
    glfwTerminate();
}

void lumen::Application::_framebuffer_resize_callback(GLFWwindow *window, [[maybe_unused]] int width, [[maybe_unused]] int height)
{
    auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

    if (app && app->_renderer) {
        app->_renderer->notify_framebuffer_resized();
    }
}
