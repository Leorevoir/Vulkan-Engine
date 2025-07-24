#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Maths/Vector.hpp>

#include <VKE/Graphics/Descriptor/VulkanDescriptorSet.hpp>
#include <VKE/Graphics/Device/VulkanDevice.hpp>
#include <VKE/Graphics/Pipelines/VulkanPipelines.hpp>

namespace vke {

struct VKE_API VulkanContext {

    public:
        explicit VulkanContext(priv::VulkanDevice *device, maths::Vector2u *screen_size);

        /** setters */
        void setPipelines(priv::VulkanPipelines *pipelines);
        void setRenderPass(VkRenderPass *render_pass);
        void setDescriptorSet(priv::VulkanDescriptorSet *descriptor_set);

        /** getters */
        VkDevice &getLogicalDevice();
        VkPhysicalDevice &getPhysicalDevice();
        VkRenderPass *getRenderPass();
        maths::Vector2u *getScreenSize();
        priv::VulkanDevice *getDevice();
        priv::VulkanPipelines *getPipelines();
        priv::VulkanDescriptorSet *getDescriptorSet();

    private:
        priv::VulkanDevice *_device = VKE_NULLPTR;
        priv::VulkanPipelines *_pipelines = VKE_NULLPTR;
        priv::VulkanDescriptorSet *_descriptor_set = VKE_NULLPTR;
        maths::Vector2u *_screen_size = VKE_NULLPTR;

        VkRenderPass *_render_pass = VKE_NULLPTR;
};

}// namespace vke
