#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>

#include <VKE/Graphics/Device/VulkanDevice.hpp>
#include <VKE/Maths/Vector.hpp>

namespace vke {

struct VKE_API VulkanContext {

        priv::VulkanDevice *_device = VKE_NULLPTR;
        VkCommandBuffer _copy_cmd = VKE_NULLPTR;
        VkCommandPool _cmd_pool = VKE_NULLPTR;
        VkPipelineLayout *_pipeline_layout = VKE_NULLPTR;
        VkPipelineCache _pipeline_cache = VKE_NULLPTR;
        VkRenderPass _render_pass = VKE_NULLPTR;
        VkQueue _queue = VKE_NULLPTR;
        maths::Vector2u *_screen_size;

        explicit VulkanContext(priv::VulkanDevice *device, maths::Vector2u *screen_size);

        VkDevice &getDevice();
        VkPhysicalDevice &getPhysicalDevice();
};

}// namespace vke
