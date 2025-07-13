#pragma once

#include <Lib/Maths/Vector.hpp>

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Types.hpp>
#include <VKE/Window/Window.hpp>

namespace vke {

namespace priv {

struct VKE_HIDDEN SwapchainBuffers {
        VkImage _image;
        VkImageView _view;
};

struct VKE_HIDDEN SwapChainColor {
        VkFormat _format;
        VkColorSpaceKHR _space;
};

class VKE_HIDDEN Swapchain final : public NonMovable
{
    public:
        void init(Window &window);
        void create(maths::Vector2u &size, bool vsync = false);
        void connect(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device);

        VkResult next(VkSemaphore present_semaphore, u32 &image_index);
        VkResult queue(VkQueue queue, u32 image_index, VkSemaphore wait_semaphore = VKE_NULL_PTR);

        void destroy();

        SwapChainColor color;

    private:
        u32 _queue_index = 0;
        u32 _image_count = 0;

        VkDevice _device = VKE_NULL_PTR;
        VkInstance _instance = VKE_NULL_PTR;
        VkSurfaceKHR _surface = VKE_NULL_PTR;
        VkPhysicalDevice _physical_device = VKE_NULL_PTR;

        VkSwapchainKHR _swapchain = VKE_NULL_PTR;

        std::vector<VkImage> _images;
        std::vector<SwapchainBuffers> _buffers;

        PFN_vkQueuePresentKHR _QueuePresentKHR;
        PFN_vkCreateSwapchainKHR _CreateSwapchainKHR;
        PFN_vkDestroySwapchainKHR _DestroySwapchainKHR;
        PFN_vkAcquireNextImageKHR _AcquireNextImageKHR;
        PFN_vkGetSwapchainImagesKHR _GetSwapchainImagesKHR;
        PFN_vkGetPhysicalDeviceSurfaceSupportKHR _GetPhysicalDeviceSurfaceSupportKHR;
        PFN_vkGetPhysicalDeviceSurfaceFormatsKHR _GetPhysicalDeviceSurfaceFormatsKHR;
        PFN_vkGetPhysicalDeviceSurfacePresentModesKHR _GetPhysicalDeviceSurfacePresentModesKHR;
        PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR _GetPhysicalDeviceSurfaceCapabilitiesKHR;

        void _destroy_buffer();
};

}// namespace priv

}// namespace vke
