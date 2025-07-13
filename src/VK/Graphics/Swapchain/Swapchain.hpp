#pragma once

#include <Lib/Maths/Vector.hpp>

#include <VK/Backend.hpp>
#include <VK/Macros.hpp>
#include <VK/Types.hpp>
#include <VK/Window/Window.hpp>

namespace vk {

namespace priv {

struct VK_HIDDEN SwapchainBuffers {
        VkImage _image;
        VkImageView _view;
};

struct VK_HIDDEN SwapChainColor {
        VkFormat _format;
        VkColorSpaceKHR _space;
};

}// namespace priv

class VK_HIDDEN Swapchain final : public NonMovable
{
    public:
        void init(Window &window);
        void create(maths::Vector2u &size, bool vsync = false);
        void connect(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device);

        VkResult next(VkSemaphore present_semaphore, u32 &image_index);
        VkResult queue(VkQueue queue, u32 image_index, VkSemaphore wait_semaphore = VK_NULL_PTR);

        void destroy();

        priv::SwapChainColor color;

    private:
        u32 _queue_index = 0;
        u32 _image_count = 0;

        VkDevice _device = VK_NULL_PTR;
        VkInstance _instance = VK_NULL_PTR;
        VkSurfaceKHR _surface = VK_NULL_PTR;
        VkPhysicalDevice _physical_device = VK_NULL_PTR;

        VkSwapchainKHR _swapchain = VK_NULL_PTR;

        std::vector<VkImage> _images;
        std::vector<priv::SwapchainBuffers> _buffers;

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

}// namespace vk
