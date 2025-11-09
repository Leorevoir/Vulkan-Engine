#include <vulkan_backend/core/Device.hpp>
#include <vulkan_backend/core/Instance.hpp>
#include <vulkan_backend/resources/ImageView.hpp>
#include <vulkan_backend/surface/Surface.hpp>
#include <vulkan_backend/surface/SwapChain.hpp>
#include <vulkan_backend/utils/Result.hpp>
#include <vulkan_backend/utils/VulkanUtils.hpp>

#include <algorithm>
#include <limits>

#include <memory>

/**
* public
*/

lumen::SwapChain::SwapChain(Instance &instance, Device &device, Surface &surface, GLFWwindow *window)
    : _instance(instance), _device(device), _surface(surface)
{
    _create_swap_chain(window);
    _create_image_views();
}

lumen::SwapChain::~SwapChain()
{
    _imageViews.clear();
    vkDestroySwapchainKHR(_device.logicalDevice(), _swapChain, nullptr);
}

/**
 * getters
 */

VkSwapchainKHR lumen::SwapChain::handle() const noexcept
{
    return _swapChain;
}

VkFormat lumen::SwapChain::getImageFormat() const noexcept
{
    return _imageFormat;
}

VkExtent2D lumen::SwapChain::getExtent() const noexcept
{
    return _extent;
}

const std::vector<std::unique_ptr<lumen::ImageView>> &lumen::SwapChain::getImageViews() const noexcept
{
    return _imageViews;
}

uint32_t lumen::SwapChain::getImageCount() const noexcept
{
    return static_cast<uint32_t>(_images.size());
}

/**
* private
*/

void lumen::SwapChain::_create_swap_chain(GLFWwindow *window)
{
    SwapChainSupportDetails swapChainSupport = utils::query_swapchain_support(_device.physicalDevice(), _surface.handle());

    VkSurfaceFormatKHR surfaceFormat = _choose_swap_surface_format(swapChainSupport.formats);
    VkPresentModeKHR presentMode = _choose_swap_present_mode(swapChainSupport.presentModes);
    _extent = _choose_swap_extent(swapChainSupport.capabilities, window);
    _imageFormat = surfaceFormat.format;

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface.handle();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = _imageFormat;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = _extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = _device.getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    vk_check(vkCreateSwapchainKHR(_device.logicalDevice(), &createInfo, nullptr, &_swapChain), "failed to create swap chain!");

    vkGetSwapchainImagesKHR(_device.logicalDevice(), _swapChain, &imageCount, nullptr);
    _images.resize(imageCount);
    vkGetSwapchainImagesKHR(_device.logicalDevice(), _swapChain, &imageCount, _images.data());
}

void lumen::SwapChain::_create_image_views()
{
    _imageViews.resize(_images.size());
    for (size_t i = 0; i < _images.size(); ++i) {
        _imageViews[i] = std::make_unique<ImageView>(_device, _images[i], _imageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }
}

VkSurfaceFormatKHR lumen::SwapChain::_choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR lumen::SwapChain::_choose_swap_present_mode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D lumen::SwapChain::_choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
