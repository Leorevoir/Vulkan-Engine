#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Device;

class ImageView
{
    public:
        ImageView(Device &device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
        ~ImageView() noexcept;

        VkImageView handle() const noexcept;

    private:
        VkImageView _imageView;
        Device &_device;
};

}// namespace lumen
