#include "vulkan_backend/resources/Framebuffer.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/pipeline/RenderPass.hpp"
#include "vulkan_backend/resources/ImageView.hpp"
#include "vulkan_backend/utils/Result.hpp"

/**
 * static private
 */

namespace {

static inline VkFramebuffer __create_framebuffer_handle(lumen::Device &device, const lumen::RenderPass &renderPass,
    const lumen::ImageView &imageView, VkExtent2D extent)
{
    VkImageView attachments[] = {imageView.handle()};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass.handle();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    VkFramebuffer framebuffer_handle;
    vk_check(vkCreateFramebuffer(device.logicalDevice(), &framebufferInfo, nullptr, &framebuffer_handle), "failed to create framebuffer!");
    return framebuffer_handle;
}
}// namespace

/**
 * public
 */

lumen::Framebuffer::Framebuffer(Device &device, const RenderPass &renderPass, const ImageView &imageView, VkExtent2D extent)
    : VulkanObject(device, __create_framebuffer_handle(device, renderPass, imageView, extent))
{
    /* __ctor__ */
}

lumen::Framebuffer::~Framebuffer()
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(_device.logicalDevice(), _handle, nullptr);
    }
}
