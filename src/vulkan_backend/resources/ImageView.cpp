#include "vulkan_backend/resources/ImageView.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"

/**
 * static private
 */

namespace {

static inline VkImageView __create_image_view_handle(lumen::Device &device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    createInfo.subresourceRange.aspectMask = aspectFlags;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    VkImageView image_view_handle;
    vk_check(vkCreateImageView(device.logicalDevice(), &createInfo, nullptr, &image_view_handle), "failed to create image view!");
    return image_view_handle;
}

}// namespace

/**
* public
*/

lumen::ImageView::ImageView(Device &device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
    : VulkanObject(device, __create_image_view_handle(device, image, format, aspectFlags))
{
    /* __ctor__ */
}

lumen::ImageView::~ImageView() noexcept
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyImageView(_device.logicalDevice(), _handle, nullptr);
    }
}
