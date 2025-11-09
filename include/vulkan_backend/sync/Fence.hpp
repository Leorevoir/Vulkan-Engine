#pragma once

#include "vulkan_backend/core/VulkanObject.hpp"

namespace lumen {

class Fence : public VulkanObject<VkFence>
{
    public:
        Fence(Device &device, bool signaled = false);
        ~Fence() override;

        void wait() const noexcept;
        void reset() const noexcept;
};

}// namespace lumen
