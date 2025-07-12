#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #error "Windows support is not implemented yet."
#elif defined(__APPLE__)
    #error "macOS support is not implemented yet."
#endif

#define VK_FORCE_CLEAR_COLOR 0x00aade87

#include <Lib/Maths/Vector.hpp>

namespace vk {

namespace detail {

struct VK_BackendWindow {
    public:
        virtual ~VK_BackendWindow() = default;

        bool _closed = true;

        virtual void display() const = 0;
        virtual void event() = 0;
};

}// namespace detail

}// namespace vk
