#pragma once

#include "vulkan_backend/core/VulkanObject.hpp"

namespace lumen {

class Semaphore : public VulkanObject<VkSemaphore>
{
    public:
        explicit Semaphore(Device &device);
        ~Semaphore() noexcept override;
};

}// namespace lumen
