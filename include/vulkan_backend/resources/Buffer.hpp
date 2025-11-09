#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Buffer
{
    public:
        Buffer();
        ~Buffer();

        VkBuffer handle() const;

    private:
        VkBuffer _buffer = VK_NULL_HANDLE;
};

}// namespace lumen
