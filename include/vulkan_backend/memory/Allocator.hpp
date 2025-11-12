#pragma once

#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/memory/MemoryPool.hpp>

#include <memory>
#include <optional>
#include <vector>

// clang-format off

namespace lumen {

class Device;

struct Allocation {
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDeviceSize offset = 0;
};

class Allocator
{
    public:
        Allocator(Device &device);
        ~Allocator() = default;

        Allocator(const Allocator &) = delete;
        Allocator &operator=(const Allocator &) = delete;

        std::optional<Allocation> allocate(const VkMemoryRequirements &requirements, VkMemoryPropertyFlags properties);

    private:
        uint32_t _find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties);

        std::optional<Allocation> _allocate_new_pool(
            const VkMemoryRequirements &requirements,
            const uint32_t memory_type_index,
            std::vector<std::unique_ptr<MemoryPool>> &pool_list);

        Device &_device;
        VkPhysicalDeviceMemoryProperties _memoryProperties;
        std::vector<std::vector<std::unique_ptr<MemoryPool>>> _pools;
};

}// namespace lumen
