#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Types.hpp>

#include <VKE/Graphics/Device/VulkanBuffer.hpp>

namespace vke {

namespace priv {

struct VKE_API BufferMemoryPair {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
};

struct VKE_API VulkanDevice final {

    public:
        VkPhysicalDevice _physicalDevice;
        VkDevice _logicalDevice;
        VkPhysicalDeviceProperties _properties;
        VkPhysicalDeviceFeatures _features;
        VkPhysicalDeviceFeatures _enabledFeatures;
        VkPhysicalDeviceMemoryProperties _memoryProperties;

        std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
        std::vector<std::string> _supportedExtensions;
        std::vector<BufferMemoryPair> __allocated_buffers;

        VkCommandPool _command_pool = VKE_NULLPTR;

        struct {
                uint32_t graphics;
                uint32_t compute;
                uint32_t transfer;
        } _queueFamilyIndices;

        explicit VulkanDevice(VkPhysicalDevice physical_device);
        ~VulkanDevice();

        /** getters */
        u32 getMemoryType(u32 type_bits, VkMemoryPropertyFlags properties, u32 *memory_type_index = VKE_NULLPTR) const;
        u32 getQueueFamilyIndex(VkQueueFlagBits queue_flags) const;
        bool isExtensionSupported(std::string extension) const;

        /** create */
        VkResult createLogicalDevice(VkPhysicalDeviceFeatures enabled_features, std::vector<const char *> enabled_extensions, void *next_chain, bool use_swapchain = true,
            VkQueueFlags requested_queue_types = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

        VkResult createBuffer(VkBufferUsageFlags usage_flags, VkMemoryPropertyFlags memory_property_flags, VkDeviceSize size, VkBuffer *buffer, VkDeviceMemory *memory,
            const void *data = VKE_NULLPTR);
        VkResult createBuffer(VkBufferUsageFlags usage_flags, VkMemoryPropertyFlags memory_property_flags, VulkanBuffer *buffer, VkDeviceSize size,
            const void *data = VKE_NULLPTR);

        VkCommandPool createCommandPool(uint32_t queue_family_index, VkCommandPoolCreateFlags create_flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, VkCommandPool pool, bool begin = false);
        VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, bool begin = false);
};

bool getSuportedDepthFormat(VkPhysicalDevice physical_device, VkFormat *depth_format);

}// namespace priv

}// namespace vke
