#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>

#include <VKE/Graphics/Device/VulkanDevice.hpp>
#include <VKE/Maths/Vector.hpp>

namespace vke {

struct VKE_API VulkanContext {

        priv::VulkanDevice *_device = VKE_NULL_PTR;
        VkCommandBuffer _copy_cmd = VKE_NULL_PTR;
        VkCommandPool _cmd_pool = VKE_NULL_PTR;
        VkPipelineLayout *_pipeline_layout = VKE_NULL_PTR;
        VkPipelineCache _pipeline_cache = VKE_NULL_PTR;
        VkRenderPass _render_pass = VKE_NULL_PTR;
        VkQueue _queue = VKE_NULL_PTR;
        maths::Vector2u *_screen_size;

        explicit VulkanContext(priv::VulkanDevice *device, maths::Vector2u *screen_size);

        inline VkDevice &getDevice()
        {
            return _device->_logicalDevice;
        }
        inline VkPhysicalDevice &getPhysicalDevice()
        {
            return _device->_physicalDevice;
        }
};

}// namespace vke
