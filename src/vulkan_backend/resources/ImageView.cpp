#include "vulkan_backend/resources/ImageView.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/utils/Result.hpp"

/**
* public
*/

lumen::ImageView::ImageView(Device &device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) : _device(device)
{
    VkImageViewCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = aspectFlags;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    vk_check(vkCreateImageView(_device.logicalDevice(), &createInfo, nullptr, &_imageView), "failed to create image view!");
}

lumen::ImageView::~ImageView() noexcept
{
    vkDestroyImageView(_device.logicalDevice(), _imageView, nullptr);
}

/**
* getters
*/

VkImageView lumen::ImageView::handle() const noexcept
{
    return _imageView;
}
