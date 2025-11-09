#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Device;
class ImageView;
class RenderPass;

class Framebuffer
{
    public:
        Framebuffer(Device &device, const RenderPass &renderPass, const ImageView &imageView, VkExtent2D extent);
        ~Framebuffer();

        VkFramebuffer handle() const noexcept;

    private:
        VkFramebuffer _framebuffer;
        Device &_device;
};

}// namespace lumen
