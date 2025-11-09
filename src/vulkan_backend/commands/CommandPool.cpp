#include "vulkan_backend/commands/CommandPool.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"

/**
* public
*/

lumen::CommandPool::CommandPool(Device &device, uint32_t queueFamilyIndex) : _device(device)
{
    VkCommandPoolCreateInfo poolInfo{};

    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    vk_check(vkCreateCommandPool(_device.logicalDevice(), &poolInfo, nullptr, &_commandPool), "failed to create command pool!");
}

lumen::CommandPool::~CommandPool()
{
    vkDestroyCommandPool(_device.logicalDevice(), _commandPool, nullptr);
}

VkCommandPool lumen::CommandPool::handle() const
{
    return _commandPool;
}
