#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Types.hpp>

namespace vke {

namespace priv {

struct VKE_HIDDEN Buffer final {

    public:
        VkDevice _device;
        VkDeviceSize _size = 0;
        VkDeviceSize _alignment = 0;
        VkBuffer _buffer = VKE_NULL_PTR;
        VkDeviceMemory _memory = VKE_NULL_PTR;
        VkDescriptorBufferInfo _descriptorInfo;

        VkBufferUsageFlags _usageFlags = 0;
        VkMemoryPropertyFlags _memoryPropertyFlags = 0;

        void *_mapped = VKE_NULL_PTR;

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void unmap();
        VkResult bind(VkDeviceSize offset = 0);
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        void copyTo(void *data, VkDeviceSize size);

        void setupDescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        void destroy();
};

}// namespace priv

}// namespace vke
