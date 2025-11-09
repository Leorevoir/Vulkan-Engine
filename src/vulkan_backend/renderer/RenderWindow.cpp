#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/renderer/RenderWindow.hpp>

#include <stdexcept>

/**
* public
*/

lumen::RenderWindow::RenderWindow(const char *title, uint32_t width, uint32_t height)
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    _window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr);
    if (!_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, _glfw_framebuffer_resize_callback);
}

lumen::RenderWindow::~RenderWindow()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

/**
 * setters
 */

void lumen::RenderWindow::poll_events() const
{
    glfwPollEvents();
}

void lumen::RenderWindow::set_framebuffer_resize_callback(const FramebufferResizeCallback &callback)
{
    _resizeCallback = callback;
}

/**
 * getters
 */

bool lumen::RenderWindow::should_close() const
{
    return glfwWindowShouldClose(_window);
}

GLFWwindow *lumen::RenderWindow::get_native_handle() const
{
    return _window;
}

/**
* private static
*/

void lumen::RenderWindow::_glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height)
{
    auto renderWindow = static_cast<RenderWindow *>(glfwGetWindowUserPointer(window));
    if (renderWindow && renderWindow->_resizeCallback) {
        renderWindow->_resizeCallback(width, height);
    }
}
