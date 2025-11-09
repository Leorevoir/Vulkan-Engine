#pragma once

#include <vulkan/vulkan.h>

namespace lumen {

class Device;
class SwapChain;

class RenderPass
{
    public:
        RenderPass(Device &device, const SwapChain &swapChain);
        ~RenderPass() noexcept;

        VkRenderPass handle() const noexcept;

    private:
        VkRenderPass _renderPass;
        Device &_device;
};

}// namespace lumen
