#pragma once

#include "vulkan_backend/core/VulkanObject.hpp"

namespace lumen {

class ImageView;
class RenderPass;

class Framebuffer : public VulkanObject<VkFramebuffer>
{
    public:
        Framebuffer(Device &device, const RenderPass &renderPass, const ImageView &imageView, VkExtent2D extent);
        ~Framebuffer() override;
};

}// namespace lumen
