#include "vulkan_backend/commands/CommandBuffer.hpp"
#include "vulkan_backend/commands/CommandPool.hpp"

#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"

/**
* public
*/

lumen::CommandBuffer::CommandBuffer(Device &device, const CommandPool &commandPool) : _device(device)
{
    VkCommandBufferAllocateInfo allocInfo{};

    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.handle();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    vk_check(vkAllocateCommandBuffers(_device.logicalDevice(), &allocInfo, &_commandBuffer), "failed to allocate command buffer!");
}

void lumen::CommandBuffer::begin()
{
    VkCommandBufferBeginInfo beginInfo{};

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vk_check(vkBeginCommandBuffer(_commandBuffer, &beginInfo), "failed to begin recording command buffer!");
}

void lumen::CommandBuffer::end()
{
    vk_check(vkEndCommandBuffer(_commandBuffer), "failed to record command buffer!");
}

void lumen::CommandBuffer::reset()
{
    vkResetCommandBuffer(_commandBuffer, 0);
}

VkCommandBuffer lumen::CommandBuffer::handle() const
{
    return _commandBuffer;
}
