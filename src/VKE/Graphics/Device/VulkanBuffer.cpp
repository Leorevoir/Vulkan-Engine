#include <VKE/Graphics/Device/VulkanBuffer.hpp>

/**
* public
*/

VkResult vke::VulkanBuffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    return vkMapMemory(_device, _memory, offset, size, 0, &_mapped);
}

void vke::VulkanBuffer::unmap()
{
    if (_mapped) {
        vkUnmapMemory(_device, _memory);
        _mapped = VKE_NULLPTR;
    }
}

VkResult vke::VulkanBuffer::bind(VkDeviceSize offset)
{
    return vkBindBufferMemory(_device, _buffer, _memory, offset);
}

void vke::VulkanBuffer::copyTo(void *data, VkDeviceSize size)
{
    if (_mapped) {
        memcpy(_mapped, data, size);
    }
}

void vke::VulkanBuffer::setupDescriptorInfo(VkDeviceSize size, VkDeviceSize offset)
{
    _descriptorInfo.buffer = _buffer;
    _descriptorInfo.offset = offset;
    _descriptorInfo.range = size;
}

VkResult vke::VulkanBuffer::invalidate(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange range = {};

    range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    range.memory = _memory;
    range.offset = offset;
    range.size = size;
    return vkInvalidateMappedMemoryRanges(_device, 1, &range);
}

VkResult vke::VulkanBuffer::flush(VkDeviceSize size, VkDeviceSize offset)
{
    VkMappedMemoryRange range = {};

    range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    range.memory = _memory;
    range.offset = offset;
    range.size = size;
    return vkFlushMappedMemoryRanges(_device, 1, &range);
}

void vke::VulkanBuffer::destroy()
{
    if (_buffer) {
        vkDestroyBuffer(_device, _buffer, nullptr);
        _buffer = VKE_NULLPTR;
    }
    if (_memory) {
        vkFreeMemory(_device, _memory, nullptr);
        _memory = VKE_NULLPTR;
    }
    _size = 0;
    _alignment = 0;
    _usageFlags = 0;
    _memoryPropertyFlags = 0;
}
