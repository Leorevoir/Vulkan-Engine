#pragma once

#include "../utils/VulkanUtils.hpp"
#include <memory>
#include <vulkan/vulkan.h>

namespace lumen {

class Instance;
class Surface;
class CommandQueue;

class Device
{
    public:
        Device(Instance &instance, Surface &surface);
        ~Device() noexcept;

        VkDevice logicalDevice() const noexcept;
        VkPhysicalDevice physicalDevice() const noexcept;

        CommandQueue &getGraphicsQueue() const noexcept;
        CommandQueue &getPresentQueue() const noexcept;

        const QueueFamilyIndices &getQueueFamilyIndices() const noexcept;

    private:
        void _pick_physical_device(Instance &instance, Surface &surface);
        void _create_logical_device(Surface &surface);
        bool _is_device_suitable(VkPhysicalDevice device, Surface &surface);

        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _device = VK_NULL_HANDLE;

        QueueFamilyIndices _indices;

        std::unique_ptr<CommandQueue> _graphicsQueue;
        std::unique_ptr<CommandQueue> _presentQueue;
};

}// namespace lumen
