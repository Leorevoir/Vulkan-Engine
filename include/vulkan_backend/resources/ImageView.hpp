#pragma once

#include "vulkan_backend/core/VulkanObject.hpp"

namespace lumen {

class ImageView : public VulkanObject<VkImageView>
{
    public:
        ImageView(Device &device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        ~ImageView() noexcept override;
};

}// namespace lumen
