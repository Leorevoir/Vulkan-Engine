#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #error "Windows support is not implemented yet."
#endif

#define VKE_FORCE_CLEAR_COLOR 0x00aade87

#include <Lib/Maths/Vector.hpp>
#include <VKE/Backend.hpp>

namespace vke {

namespace detail {

class VKE_HIDDEN VKE_BackendWindow
{
    public:
        VKE_INLINE constexpr VKE_BackendWindow(const maths::Vector2u &size, const std::string &title) : _size(size), _title(title)
        {
            /* __ctor__ */
        }

        virtual ~VKE_BackendWindow() = default;

        maths::Vector2u _size;
        std::string _title;
        bool _closed = true;

        virtual void event() = 0;
        virtual void flush() = 0;
        virtual void createVulkanSurface(VkInstance instance, VkSurfaceKHR &out_surface) = 0;
};

}// namespace detail

}// namespace vke
