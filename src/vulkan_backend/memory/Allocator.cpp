#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/memory/Allocator.hpp>

/**
 * static helpers
 */

namespace {

static constexpr VkDeviceSize DEFAULT_POOL_SIZE = 256 * 1024 * 1024;///<< 256 MB

}//namespace

/**
* public
*/

lumen::Allocator::Allocator(Device &device) : _device(device)
{
    vkGetPhysicalDeviceMemoryProperties(_device.physicalDevice(), &_memoryProperties);
    _pools.resize(_memoryProperties.memoryTypeCount);
}

std::optional<lumen::Allocation> lumen::Allocator::allocate(const VkMemoryRequirements &requirements, VkMemoryPropertyFlags properties)
{
    const uint32_t memory_type_index = _find_memory_type(requirements.memoryTypeBits, properties);
    auto &pool_list = _pools[memory_type_index];

    for (const auto &pool : pool_list) {
        const VkDeviceSize offset = pool->allocate(requirements.size, requirements.alignment);

        if (offset != MemoryPool::InvalidAllocation) {
            return Allocation{pool->handle(), offset};
        }
    }
    return _allocate_new_pool(requirements, memory_type_index, pool_list);
}

/**
* private
*/
// clang-format off

uint32_t lumen::Allocator::_find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties)
{
    for (uint32_t i = 0; i < _memoryProperties.memoryTypeCount; ++i) {
        if ((type_filter & (1 << i)) && (_memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type!");
}

std::optional<lumen::Allocation> lumen::Allocator::_allocate_new_pool(
    const VkMemoryRequirements &requirements,
    const uint32_t memory_type_index,
    std::vector<std::unique_ptr<MemoryPool>> &pool_list
)
{
    const VkDeviceSize new_pool_size = std::max(DEFAULT_POOL_SIZE, requirements.size);
    auto new_pool = std::make_unique<MemoryPool>(_device, memory_type_index, new_pool_size);
    const VkDeviceSize offset = new_pool->allocate(requirements.size, requirements.alignment);

    if (offset != MemoryPool::InvalidAllocation) {
        const Allocation alloc = {new_pool->handle(), offset};

        pool_list.push_back(std::move(new_pool));
        return alloc;
    }

    return std::nullopt;
}
