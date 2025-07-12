#include <VK/Error.hpp>
#include <VK/Window/Window.hpp>

/**
* public
*/

#include <iostream>

vk::Window::Window(const maths::Vector2u &size, const VK_UNUSED std::string &title)
{
    std::cout << "creating window with size: " << size << std::endl;
    if (!VK_MATHS_NOT_NULL(size.x) && !VK_MATHS_NOT_NULL(size.y)) {
        throw vk::exception::InvalidArgument("vk::Window::Window", "Window size must be > 0");
    }
#if defined(__linux__)
    _vk_backend = std::make_unique<detail::VK_X11Window>(size, title);
#else
    throw vk::exception::NotImplemented("vk::Window::Window", "Platform not supported");
#endif
}

vk::Window::~Window()
{
    /* __dtor__ */
}

bool vk::Window::shouldClose() const
{
    return _vk_backend->_closed == true;
}

void vk::Window::display()
{
    _vk_backend->display();
}

void vk::Window::event()
{
    _vk_backend->event();
}
