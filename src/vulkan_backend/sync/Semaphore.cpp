#include "vulkan_backend/sync/Semaphore.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"

namespace {

static inline VkSemaphore __create_semaphore_handle(lumen::Device &device)
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkSemaphore semaphore_handle;

    vk_check(vkCreateSemaphore(device.logicalDevice(), &semaphoreInfo, nullptr, &semaphore_handle), "failed to create semaphore!");

    return semaphore_handle;
}

}// namespace

/**
* public
*/

lumen::Semaphore::Semaphore(Device &device) : VulkanObject(device, __create_semaphore_handle(device))
{
    /* __ctor__ */
}

lumen::Semaphore::~Semaphore() noexcept
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroySemaphore(_device.logicalDevice(), _handle, nullptr);
    }
}
