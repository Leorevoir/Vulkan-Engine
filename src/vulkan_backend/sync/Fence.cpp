#include "vulkan_backend/sync/Fence.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"

/**
 * public
 */

lumen::Fence::Fence(Device &device, bool signaled) : _device(device)
{
    VkFenceCreateInfo fenceInfo{};

    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    if (signaled) {
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    vk_check(vkCreateFence(_device.logicalDevice(), &fenceInfo, nullptr, &_fence), "failed to create fence!");
}

lumen::Fence::~Fence()
{
    vkDestroyFence(_device.logicalDevice(), _fence, nullptr);
}

/**
* setters
*/

void lumen::Fence::wait() const noexcept
{
    vkWaitForFences(_device.logicalDevice(), 1, &_fence, VK_TRUE, UINT64_MAX);
}

void lumen::Fence::reset() const noexcept
{
    vkResetFences(_device.logicalDevice(), 1, &_fence);
}

/**
* getters
*/

VkFence lumen::Fence::handle() const noexcept
{
    return _fence;
}
