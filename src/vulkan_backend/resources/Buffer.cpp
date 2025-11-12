#include <vulkan_backend/core/GraphicsContext.hpp>
#include <vulkan_backend/resources/Buffer.hpp>
#include <vulkan_backend/utils/Result.hpp>

#include <cstring>

/**
* static helpers
*/

namespace {

static inline VkBuffer create_buffer_handle(lumen::Device &device, VkDeviceSize size, VkBufferUsageFlags usage)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer buffer;
    vk_check(vkCreateBuffer(device.logicalDevice(), &bufferInfo, nullptr, &buffer), "Failed to create buffer!");
    return buffer;
}

}// namespace

/**
* public
*/

lumen::Buffer::Buffer(GraphicsContext &context, VkDeviceSize size, VkBufferUsageFlags buffer_usage, VkMemoryPropertyFlags memory_properties)
    : VulkanObject(context.get_device(), create_buffer_handle(context.get_device(), size, buffer_usage)), _context(context), _size(size)
{

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(_device.logicalDevice(), _handle, &memRequirements);

    auto allocationOpt = _context.get_allocator().allocate(memRequirements, memory_properties);
    if (!allocationOpt.has_value()) {
        throw std::runtime_error("Failed to allocate memory for buffer!");
    }
    _allocation = allocationOpt.value();

    vk_check(vkBindBufferMemory(_device.logicalDevice(), _handle, _allocation.memory, _allocation.offset), "Failed to bind buffer memory!");
}

lumen::Buffer::~Buffer()
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyBuffer(_device.logicalDevice(), _handle, nullptr);
    }
}

void *lumen::Buffer::map()
{
    void *data;
    vkMapMemory(_device.logicalDevice(), _allocation.memory, _allocation.offset, _size, 0, &data);
    return data;
}

void lumen::Buffer::unmap()
{
    vkUnmapMemory(_device.logicalDevice(), _allocation.memory);
}

void lumen::Buffer::write(const void *data, VkDeviceSize size)
{
    void *mapped = map();

    std::memcpy(mapped, data, size);
    unmap();
}
