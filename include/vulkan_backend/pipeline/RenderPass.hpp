#pragma once

#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/core/VulkanObject.hpp>

namespace lumen {

class SwapChain;

class RenderPass : public VulkanObject<VkRenderPass>
{
    public:
        RenderPass(Device &device, const SwapChain &swapChain);
        ~RenderPass() noexcept override;
};

}// namespace lumen
