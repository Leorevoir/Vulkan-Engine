#include <VK/Error.hpp>
#include <VK/Window/Platform/X11.hpp>

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

void vk::detail::VK_X11Window::event()
{
    if (this->_closed) {
        return;
    }

    if (XPending(_display) == 0) {
        return;
    }

    XNextEvent(_display, &_xEvent);

    switch (_xEvent.type) {
        case ClientMessage:
            if (_xEvent.xclient.data.l[0] == static_cast<i32>(_xDeleteWindowAtom)) {
                _destroy();
            }
            break;
        default:
            break;
    }
}

void vk::detail::VK_X11Window::display() const
{
    if (this->_closed) {
        return;
    }
}

/**
* private
*/

void vk::detail::VK_X11Window::_create()
{
    _destroy();

    this->_closed = false;
    _display = XOpenDisplay(0);

    if (!_display) {
        throw vk::exception::RuntimeError("vk::detail::VK_X11Window::VK_X11Window", "Failed to open X display");
    }

    _xRootWindow = XDefaultRootWindow(_display);
    _xWindow = XCreateSimpleWindow(_display, _xRootWindow, 0, 0, _size.x, _size.y, 0, 0, VK_FORCE_CLEAR_COLOR);

    XMapWindow(_display, _xWindow);

    _xDeleteWindowAtom = XInternAtom(_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(_display, _xWindow, &_xDeleteWindowAtom, 1);
}

void vk::detail::VK_X11Window::_destroy()
{
    if (this->_closed) {
        return;
    }

    this->_closed = true;

    if (_display == nullptr) {
        return;
    }

    XDestroyWindow(_display, _xWindow);
    XCloseDisplay(_display);
}
