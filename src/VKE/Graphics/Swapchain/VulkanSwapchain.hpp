#pragma once

#include <VKE/Graphics/Swapchain/SwapchainBase.hpp>
#include <VKE/Maths/Vector.hpp>
#include <VKE/Window/Window.hpp>

namespace vke {

namespace priv {

class VKE_API VulkanSwapchain final : public SwapchainBase
{
    public:
        constexpr VulkanSwapchain() = default;
        ~VulkanSwapchain() override = default;

        void init(std::shared_ptr<Window> &window);
        void create(maths::Vector2u &size, bool vsync = false);
        void create(u32 *width, u32 *height, bool vsync = false);
        void connect(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device);

        VkResult next(VkSemaphore present_semaphore, u32 *image_index);
        VkResult queue(VkQueue queue, u32 image_index, VkSemaphore wait_semaphore = VKE_NULLPTR);

        void destroy();

    private:
        VkDevice _device = VKE_NULLPTR;
        VkInstance _instance = VKE_NULLPTR;
        VkSurfaceKHR _surface = VKE_NULLPTR;
        VkPhysicalDevice _physical_device = VKE_NULLPTR;

        VkSwapchainKHR _swapchain = VKE_NULLPTR;

        std::vector<VkImage> _images;
        void _destroy_buffer();
};

}// namespace priv

}// namespace vke
