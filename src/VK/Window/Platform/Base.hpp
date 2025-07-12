#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #error "Windows support is not implemented yet."
#endif

#define VK_FORCE_CLEAR_COLOR 0x00aade87

#include <Lib/Maths/Vector.hpp>

namespace vk {

namespace detail {

class VK_BackendWindow
{
    public:
        VK_INLINE constexpr VK_BackendWindow(const maths::Vector2u &size, const std::string &title) : _size(size), _title(title)
        {
            /* __ctor__ */
        }

        virtual ~VK_BackendWindow() = default;

        maths::Vector2u _size;
        std::string _title;
        bool _closed = true;

        virtual void event() = 0;
        virtual void display() = 0;
};

}// namespace detail

}// namespace vk
