#pragma once

#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/core/VulkanObject.hpp>

namespace lumen {

class CommandPool : public VulkanObject<VkCommandPool>
{
    public:
        CommandPool(Device &device, uint32_t queueFamilyIndex);
        ~CommandPool() override;
};

}// namespace lumen
