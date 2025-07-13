#pragma once

#include <VKE/Error.hpp>

#include <VKE/Backend.hpp>
#include <VKE/Graphics/Device/VulkanDevice.hpp>
#include <VKE/Graphics/Swapchain/VulkanSwapchain.hpp>
#include <VKE/Macros.hpp>

namespace vke {

namespace priv {

class VKE_HIDDEN VulkanEngineBase
{
    public:
        VulkanEngineBase() = default;
        virtual ~VulkanEngineBase() = default;

        void init_vulkan(Window &window);

    protected:
        virtual void destroy();

        bool _is_running = false;

        u32 _selectGPUIndex = 0;

        VkQueue _queue = VKE_NULL_PTR;
        VkDevice _device = VKE_NULL_PTR;
        VkInstance _instance = VKE_NULL_PTR;
        VkRenderPass _renderPass = VKE_NULL_PTR;
        VkCommandPool _commandPool = VKE_NULL_PTR;
        VkPipelineCache _pipelineCache = VKE_NULL_PTR;
        VkDescriptorPool _descriptorPool = VKE_NULL_PTR;
        VkPhysicalDevice _physicalDevice = VKE_NULL_PTR;

        VkPhysicalDeviceProperties _deviceProperties;
        VkPhysicalDeviceFeatures _deviceFeatures;
        VkPhysicalDeviceMemoryProperties _deviceMemoryProperties;

        struct VkRenderSemaphores {
                VkSemaphore present = VKE_NULL_PTR;
                VkSemaphore render = VKE_NULL_PTR;
        } _semaphores;

        std::vector<VkFence> _waitFences;
        std::vector<VkShaderModule> _shadersModules;
        std::vector<VkCommandBuffer> _commandBuffers;

        std::vector<const char *> _enabled_device_extensions;
        std::vector<const char *> _enabled_instance_extensions;

        Swapchain _swapChain;

    private:
        void _create_vulkan_instance(Window &window);
        void _pick_physical_device();
        void _create_logical_device();

        void _destroy_surface();
};

}// namespace priv

}// namespace vke
