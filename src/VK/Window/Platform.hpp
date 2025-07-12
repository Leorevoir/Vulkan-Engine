#pragma once

#include <Lib/Maths/Vector.hpp>

#include <string>
#include <unistd.h>

#define VK_FORCE_CLEAR_COLOR 0x00aade87

namespace vk {

namespace detail {

struct VK_BackendWindow {
    public:
        virtual ~VK_BackendWindow() = default;

        virtual void display() const = 0;
        virtual i32 event() const = 0;
};

#if defined(__linux__)
    #include <X11/Xlib.h>
struct VK_X11Window final : public VK_BackendWindow {

    public:
        VK_X11Window(const maths::Vector2u &size, const std::string &title);
        ~VK_X11Window();

        maths::Vector2u _size;
        std::string _title;

        void display() const override;
        VK_NODISCARD i32 event() const override;

    private:
        void _create();
        void _destroy();

    #if defined(__linux__)
        Display *_display;
        Window _xRootWindow;
        Window _xWindow;
    #endif
};
#elif defined(_WIN32) || defined(_WIN64)
    #error "Windows support is not implemented yet."
#elif defined(__APPLE__)
    #error "macOS support is not implemented yet."
#endif

}// namespace detail

}// namespace vk
