#include <vulkan_backend/commands/CommandPool.hpp>
#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/utils/Result.hpp>

/**
 * static private
 */

namespace {

static inline VkCommandPool __create_command_pool_handle(lumen::Device &device, const uint32_t queueFamilyIndex)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    VkCommandPool command_pool_handle;
    vk_check(vkCreateCommandPool(device.logicalDevice(), &poolInfo, nullptr, &command_pool_handle), "failed to create command pool!");
    return command_pool_handle;
}
}// namespace

/**
* public
*/

lumen::CommandPool::CommandPool(Device &device, uint32_t queueFamilyIndex)
    : VulkanObject(device, __create_command_pool_handle(device, queueFamilyIndex))
{
    /* __ctor__ */
}

lumen::CommandPool::~CommandPool()
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyCommandPool(_device.logicalDevice(), _handle, nullptr);
    }
}
