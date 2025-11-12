#pragma once

#include <vulkan_backend/core/VulkanObject.hpp>
#include <vulkan_backend/memory/Allocator.hpp>

namespace lumen {

class GraphicsContext;

class Buffer : public VulkanObject<VkBuffer>
{
    public:
        Buffer(GraphicsContext &context, VkDeviceSize size, VkBufferUsageFlags buffer_usage, VkMemoryPropertyFlags memory_properties);
        ~Buffer() override;

        void write(const void *data, VkDeviceSize size);
        void *map();
        void unmap();

    private:
        GraphicsContext &_context;
        Allocation _allocation;
        VkDeviceSize _size;
};

}// namespace lumen
