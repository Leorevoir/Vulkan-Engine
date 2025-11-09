#pragma once

#include <vulkan_backend/Backend.hpp>

namespace lumen {

class Image
{
    public:
        Image();
        ~Image();

        VkImage handle() const noexcept;

    private:
        VkImage _image = VK_NULL_HANDLE;
};

}// namespace lumen
