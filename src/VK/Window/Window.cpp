#include <VK/Error.hpp>
#include <VK/Window/Window.hpp>

/**
* public
*/

vk::Window::Window(const maths::Vector2u &size, const VK_UNUSED std::string &title)
{
    if (!VK_MATHS_NOT_NULL(size.x) && !VK_MATHS_NOT_NULL(size.y)) {
        throw vk::exception::InvalidArgument("vk::Window::Window", "Window size must be > 0");
    }
}

vk::Window::~Window()
{
    /* __dtor__ */
}
