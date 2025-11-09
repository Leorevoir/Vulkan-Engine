#include "vulkan_backend/resources/Framebuffer.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/pipeline/RenderPass.hpp"
#include "vulkan_backend/resources/ImageView.hpp"
#include "vulkan_backend/utils/Result.hpp"

/**
 * public
 */

lumen::Framebuffer::Framebuffer(Device &device, const RenderPass &renderPass, const ImageView &imageView, VkExtent2D extent)
    : _device(device)
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

    vk_check(vkCreateFramebuffer(_device.logicalDevice(), &framebufferInfo, nullptr, &_framebuffer), "failed to create framebuffer!");
}

lumen::Framebuffer::~Framebuffer()
{
    vkDestroyFramebuffer(_device.logicalDevice(), _framebuffer, nullptr);
}

/**
* getters
*/

VkFramebuffer lumen::Framebuffer::handle() const noexcept
{
    return _framebuffer;
}
