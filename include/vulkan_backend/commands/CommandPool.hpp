#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Device;

class CommandPool
{
    public:
        CommandPool(Device &device, uint32_t queueFamilyIndex);
        ~CommandPool();

        VkCommandPool handle() const;

    private:
        VkCommandPool _commandPool;
        Device &_device;
};

}// namespace lumen
