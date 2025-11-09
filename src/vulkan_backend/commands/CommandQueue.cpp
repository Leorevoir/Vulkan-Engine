#include "vulkan_backend/commands/CommandQueue.hpp"

/**
* public
*/

lumen::CommandQueue::CommandQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex) noexcept : _familyIndex(queueFamilyIndex)
{
    vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, &_queue);
}

/**
* getters
*/

VkQueue lumen::CommandQueue::handle() const noexcept
{
    return _queue;
}

uint32_t lumen::CommandQueue::familyIndex() const noexcept
{
    return _familyIndex;
}
