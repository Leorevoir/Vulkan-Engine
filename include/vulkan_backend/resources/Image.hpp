#pragma once

#include <vulkan/vulkan.h>

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
