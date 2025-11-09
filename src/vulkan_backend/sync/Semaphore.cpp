#include "vulkan_backend/sync/Semaphore.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"

/**
* public
*/

lumen::Semaphore::Semaphore(Device &device) : _device(device)
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    vk_check(vkCreateSemaphore(_device.logicalDevice(), &semaphoreInfo, nullptr, &_semaphore), "failed to create semaphore!");
}

lumen::Semaphore::~Semaphore() noexcept
{
    vkDestroySemaphore(_device.logicalDevice(), _semaphore, nullptr);
}

/**
* getters
*/

VkSemaphore lumen::Semaphore::handle() const noexcept
{
    return _semaphore;
}
