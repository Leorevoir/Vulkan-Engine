#pragma once

#include "../utils/VulkanUtils.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace lumen {

class Instance;
class Device;
class Surface;
class ImageView;

class SwapChain
{
    public:
        SwapChain(Instance &instance, Device &device, Surface &surface, GLFWwindow *window);
        ~SwapChain();

        VkSwapchainKHR handle() const noexcept;
        VkFormat getImageFormat() const noexcept;
        VkExtent2D getExtent() const noexcept;
        const std::vector<std::unique_ptr<ImageView>> &getImageViews() const noexcept;
        uint32_t getImageCount() const noexcept;

    private:
        void _create_swap_chain(GLFWwindow *window);
        void _create_image_views();

        VkSurfaceFormatKHR _choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> &formats);
        VkPresentModeKHR _choose_swap_present_mode(const std::vector<VkPresentModeKHR> &presentModes);
        VkExtent2D _choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window);

        VkSwapchainKHR _swapChain;
        VkFormat _imageFormat;
        VkExtent2D _extent;

        std::vector<VkImage> _images;
        std::vector<std::unique_ptr<ImageView>> _imageViews;

        Instance &_instance;
        Device &_device;
        Surface &_surface;
};

}// namespace lumen
