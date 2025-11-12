#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/memory/MemoryPool.hpp>
#include <vulkan_backend/utils/Result.hpp>

/**
 * static helpers
 */

namespace {

static inline VkDeviceMemory __create_memory_pool_handle(lumen::Device &device, const uint32_t memoryTypeIndex, const VkDeviceSize size)
{
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;

    VkDeviceMemory memory_handle;
    vk_check(vkAllocateMemory(device.logicalDevice(), &allocInfo, nullptr, &memory_handle), "Failed to allocate memory pool!");

    return memory_handle;
}

}// namespace

/**
* public
*/

lumen::MemoryPool::MemoryPool(Device &device, const uint32_t memoryTypeIndex, const VkDeviceSize size)
    : VulkanObject(device, __create_memory_pool_handle(device, memoryTypeIndex, size)), _size(size), _offset(0)
{
    /* __ctor__ */
}

lumen::MemoryPool::~MemoryPool()
{
    if (_handle != VK_NULL_HANDLE) {
        vkFreeMemory(_device.logicalDevice(), _handle, nullptr);
    }
}

VkDeviceSize lumen::MemoryPool::allocate(VkDeviceSize size, VkDeviceSize alignment)
{
    const VkDeviceSize aligned_offset = (_offset + alignment - 1) & ~(alignment - 1);

    if (aligned_offset + size > _size) {
        return InvalidAllocation;
    }

    _offset = aligned_offset + size;
    return aligned_offset;
}
