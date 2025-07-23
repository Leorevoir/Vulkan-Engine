#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Types.hpp>

namespace vke {

struct VKE_API VulkanBuffer final {

    public:
        VkDevice _device;
        VkDeviceSize _size = 0;
        VkDeviceSize _alignment = 0;
        VkBuffer _buffer = VKE_NULLPTR;
        VkDeviceMemory _memory = VKE_NULLPTR;
        VkDescriptorBufferInfo _descriptor;

        VkBufferUsageFlags _usageFlags = 0;
        VkMemoryPropertyFlags _memoryPropertyFlags = 0;

        void *_mapped = VKE_NULLPTR;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void unmap();
        VkResult bind(VkDeviceSize offset = 0);
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void copyTo(void *data, VkDeviceSize size);

        void setupDescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void destroy();
};

namespace helper {

static inline VkBufferCreateInfo create_buffer_info(VkBufferUsageFlags usage_flags, VkDeviceSize size)
{
    VkBufferCreateInfo buffer_create_info{};

    buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.usage = usage_flags;
    buffer_create_info.size = size;
    return buffer_create_info;
}

static inline VkCommandBufferAllocateInfo allocate_command_buffer(VkCommandPool command_pool, VkCommandBufferLevel level, u32 buffer_count)
{
    VkCommandBufferAllocateInfo alloc_info{};

    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool;
    alloc_info.level = level;
    alloc_info.commandBufferCount = buffer_count;
    return alloc_info;
}

}// namespace helper

}// namespace vke
