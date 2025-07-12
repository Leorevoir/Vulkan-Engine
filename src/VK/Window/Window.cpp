#include <VK/Error.hpp>
#include <VK/Window/Window.hpp>

#if defined(VK_USE_PLATFORM_XCB_KHR)
    #include <VK/Window/Platform/Xcb.hpp>
#endif

/**
* public
*/

vk::Window::Window(const maths::Vector2u &size, const VK_UNUSED std::string &title)
{
    if (!VK_MATHS_NOT_NULL(size.x) && !VK_MATHS_NOT_NULL(size.y)) {
        throw vk::exception::InvalidArgument("vk::Window::Window", "Window size must be > 0");
    }
#if defined(__linux__)
    _vk_backend = std::make_unique<detail::VK_XCBWindow>(size, title);
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

const vk::maths::Vector2u &vk::Window::getSize() const
{
    return _vk_backend->_size;
}

void vk::Window::pollEvents()
{
    _vk_backend->event();
}

void vk::Window::display()
{
    _vk_backend->display();
}
