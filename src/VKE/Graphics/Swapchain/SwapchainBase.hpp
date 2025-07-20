#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Types.hpp>

namespace vke {

namespace priv {

struct VKE_API SwapchainBuffers {
        VkImage _image;
        VkImageView _view;
};

struct VKE_API SwapChainColor {
        VkFormat _format;
        VkColorSpaceKHR _space;
};

/**
 * swapchain base class (abstract)
 */

class SwapchainBase : public NonMovable
{
    public:
        virtual ~SwapchainBase() = default;

        /** getters */
        u32 getQueueIndex() const;
        u32 getImageCount() const;
        const SwapChainColor &getColor() const;
        const std::vector<SwapchainBuffers> &getBuffers() const;

    protected:
        u32 _queue_index = 0;
        u32 _image_count = 0;

        SwapChainColor _color;
        std::vector<SwapchainBuffers> _buffers;

        /** function pointers */
        PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
        PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
        PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
        PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
        PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
        PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
        PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
        PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
        PFN_vkQueuePresentKHR fpQueuePresentKHR;
};

}// namespace priv

}// namespace vke
