#pragma once

#include <VK/Window/Platform/Base.hpp>

namespace vk {

namespace detail {

#if defined(__linux__)
    #include <X11/Xlib.h>

struct VK_X11Window final : public VK_BackendWindow {

    public:
        VK_X11Window(const maths::Vector2u &size, const std::string &title);
        ~VK_X11Window();

        void display() const override;
        void event() override;

        maths::Vector2u _size;
        std::string _title;

    private:
        void _create();
        void _destroy();

        Display *_display = nullptr;
        Window _xRootWindow;
        Window _xWindow;

        Atom _xDeleteWindowAtom;
        XEvent _xEvent;
};

#endif

}// namespace detail

}// namespace vk
