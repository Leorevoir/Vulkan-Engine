#pragma once

#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/core/VulkanObject.hpp>

namespace lumen {

class MemoryPool : public VulkanObject<VkDeviceMemory>
{
    public:
        MemoryPool(Device &device, const uint32_t memoryTypeIndex, const VkDeviceSize size);
        ~MemoryPool();

        VkDeviceSize allocate(const VkDeviceSize size, const VkDeviceSize alignment);

        static constexpr VkDeviceSize InvalidAllocation = static_cast<VkDeviceSize>(-1);

    private:
        VkDeviceSize _size = 0;
        VkDeviceSize _offset = 0;
};

}// namespace lumen
