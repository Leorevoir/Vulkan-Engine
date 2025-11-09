#include "vulkan_backend/sync/Fence.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"

namespace {

static inline VkFence __create_fence_handle(lumen::Device &device, bool signaled)
{
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    if (signaled) {
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    }

    VkFence fence_handle;

    vk_check(vkCreateFence(device.logicalDevice(), &fenceInfo, nullptr, &fence_handle), "failed to create fence!");

    return fence_handle;
}

}// namespace

/**
 * public
 */

lumen::Fence::Fence(Device &device, bool signaled) : VulkanObject(device, __create_fence_handle(device, signaled))
{
    /* __ctor__ */
}

lumen::Fence::~Fence()
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyFence(_device.logicalDevice(), _handle, nullptr);
    }
}

/**
* setters
*/

void lumen::Fence::wait() const noexcept
{
    vkWaitForFences(_device.logicalDevice(), 1, &_handle, VK_TRUE, UINT64_MAX);
}

void lumen::Fence::reset() const noexcept
{
    vkResetFences(_device.logicalDevice(), 1, &_handle);
}
