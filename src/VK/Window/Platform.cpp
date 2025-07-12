#include <VK/Error.hpp>
#include <VK/Window/Platform.hpp>
#include <X11/Xlib.h>

/**
* public
*/

vk::detail::VK_X11Window::VK_X11Window(const maths::Vector2u &size, const std::string &title) : _size(size), _title(title)
{
    _create();
}

vk::detail::VK_X11Window::~VK_X11Window()
{
    _destroy();
}

i32 vk::detail::VK_X11Window::event() const
{
    XEvent event;

    return XNextEvent(_display, &event);
}

void vk::detail::VK_X11Window::display() const
{
    XFlush(_display);
}

/**
* private
*/

void vk::detail::VK_X11Window::_create()
{
    _display = XOpenDisplay(0);

    if (!_display) {
        throw vk::exception::RuntimeError("vk::detail::VK_X11Window::VK_X11Window", "Failed to open X display");
    }

    _xRootWindow = XDefaultRootWindow(_display);
    _xWindow = XCreateSimpleWindow(_display, _xRootWindow, 0, 0, _size.x, _size.y, 0, 0, VK_FORCE_CLEAR_COLOR);

    XMapWindow(_display, _xWindow);
}

void vk::detail::VK_X11Window::_destroy()
{
    XDestroyWindow(_display, _xWindow);
    XCloseDisplay(_display);
}
