#pragma once

#include <vulkan_backend/Backend.hpp>

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
