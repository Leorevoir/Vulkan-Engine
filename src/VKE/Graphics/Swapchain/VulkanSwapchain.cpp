#include <VKE/Error.hpp>
#include <VKE/Graphics/Swapchain/VulkanSwapchain.hpp>

/**
 * helper
 */

/**
 * following macros are inspired by the LearnVulkan tutorial
 * they were made by Sascha Willems I just adapted them to my needs
 */

#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                                                                                                                         \
    {                                                                                                                                                                    \
        _##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetInstanceProcAddr(inst, "vk" #entrypoint));                                                             \
        if (_##entrypoint == NULL) {                                                                                                                                     \
            throw exception::RuntimeError("vke::priv::Swapchain", "Failed to get instance procedure address for vk" #entrypoint);                                        \
        }                                                                                                                                                                \
    }

#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                                                                                                                            \
    {                                                                                                                                                                    \
        _##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetDeviceProcAddr(dev, "vk" #entrypoint));                                                                \
        if (_##entrypoint == NULL) {                                                                                                                                     \
            throw exception::RuntimeError("vke::priv::Swapchain", "Failed to get device procedure address for vk" #entrypoint);                                          \
        }                                                                                                                                                                \
    }

/**
* public
*/

void vke::priv::VulkanSwapchain::init(Window &window)
{
    u32 queue_count;

    window.createVulkanSurface(_instance, _surface);

    vkGetPhysicalDeviceQueueFamilyProperties(_physical_device, &queue_count, VKE_NULL_PTR);
    assert(queue_count >= 1);

    std::vector<VkQueueFamilyProperties> queue_properties(queue_count);
    vkGetPhysicalDeviceQueueFamilyProperties(_physical_device, &queue_count, queue_properties.data());

    /**
     * @brief checks which queue families of the physical device can present images to the surface.
     */

    std::vector<u32> supports_presenting(queue_count);
    for (u32 i = 0; i < queue_count; ++i) {
        _GetPhysicalDeviceSurfaceSupportKHR(_physical_device, i, _surface, &supports_presenting[i]);
    }

    /**
     * @brief search for a graphics queue & a presenting queue in the queue families
     */

    u32 graphics_queue_index = UINT32_MAX;
    u32 presenting_queue_index = UINT32_MAX;

    for (u32 i = 0; i < queue_count; ++i) {

        if ((queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {

            if (graphics_queue_index == UINT32_MAX) {
                graphics_queue_index = i;
            }
        }

        if (supports_presenting[i] == VKE_TRUE) {
            graphics_queue_index = i;
            presenting_queue_index = i;
        }
    }

    if (presenting_queue_index == UINT32_MAX) {

        for (u32 i = 0; i < queue_count; ++i) {

            if (supports_presenting[i] == VKE_TRUE) {
                presenting_queue_index = i;
                break;
            }
        }
    }

    if (graphics_queue_index == UINT32_MAX || presenting_queue_index == UINT32_MAX) {
        throw exception::RuntimeError("vke::priv::VulkanSwapchain::init", "No suitable queue family found for graphics and/or presenting.");
    }

    if (graphics_queue_index != presenting_queue_index) {
        throw exception::RuntimeError("vke::priv::VulkanSwapchain::init", "Graphics and presenting queue families must be the same.");
    }

    _queue_index = graphics_queue_index;

    u32 format_count;

    VKE_ASSERT(_GetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &format_count, VKE_NULL_PTR));
    assert(format_count > 0);

    std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
    VKE_ASSERT(_GetPhysicalDeviceSurfaceFormatsKHR(_physical_device, _surface, &format_count, surface_formats.data()));

    if (format_count == 1 && surface_formats[0].format == VK_FORMAT_UNDEFINED) {
        _color._format = VK_FORMAT_B8G8R8A8_UNORM;
        _color._space = surface_formats[0].colorSpace;
        return;
    }

    for (auto &&surface : surface_formats) {
        if (surface.format == VK_FORMAT_B8G8R8A8_UNORM) {
            _color._format = surface.format;
            _color._space = surface.colorSpace;
            return;
        }
    }

    _color._format = surface_formats[0].format;
    _color._space = surface_formats[0].colorSpace;
}

void vke::priv::VulkanSwapchain::connect(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device)
{
    _instance = instance;
    _physical_device = physical_device;
    _device = device;

    GET_INSTANCE_PROC_ADDR(_instance, GetPhysicalDeviceSurfaceSupportKHR);
    GET_INSTANCE_PROC_ADDR(_instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
    GET_INSTANCE_PROC_ADDR(_instance, GetPhysicalDeviceSurfaceFormatsKHR);
    GET_INSTANCE_PROC_ADDR(_instance, GetPhysicalDeviceSurfacePresentModesKHR);
    GET_DEVICE_PROC_ADDR(_device, CreateSwapchainKHR);
    GET_DEVICE_PROC_ADDR(_device, DestroySwapchainKHR);
    GET_DEVICE_PROC_ADDR(_device, GetSwapchainImagesKHR);
    GET_DEVICE_PROC_ADDR(_device, AcquireNextImageKHR);
    GET_DEVICE_PROC_ADDR(_device, QueuePresentKHR);
}

void vke::priv::VulkanSwapchain::create(vke::maths::Vector2u &size, bool vsync)
{
    VkSwapchainKHR old_swapchain = _swapchain;
    VkSurfaceCapabilitiesKHR surface_capabilities;
    u32 presenting_mode_count;

    VKE_ASSERT(_GetPhysicalDeviceSurfaceCapabilitiesKHR(_physical_device, _surface, &surface_capabilities));
    VKE_ASSERT(_GetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &presenting_mode_count, VKE_NULL_PTR));
    assert(presenting_mode_count > 0);

    std::vector<VkPresentModeKHR> presenting_modes(presenting_mode_count);
    VKE_ASSERT(_GetPhysicalDeviceSurfacePresentModesKHR(_physical_device, _surface, &presenting_mode_count, presenting_modes.data()));

    VkExtent2D extent = {};

    if (surface_capabilities.currentExtent.width == (u32) -1) {
        extent.width = size.width;
        extent.height = size.height;
    } else {
        extent = surface_capabilities.currentExtent;
        size.width = extent.width;
        size.height = extent.height;
    }

    /**
     * @brief select a presenting mode for the swapchain & check vsync
     */

    VkPresentModeKHR sc_present_mode = VK_PRESENT_MODE_FIFO_KHR;

    if (!vsync) {

        for (u32 i = 0; i < presenting_mode_count; ++i) {

            if (presenting_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                sc_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
                break;
            }

            if (sc_present_mode != VK_PRESENT_MODE_MAILBOX_KHR && presenting_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                sc_present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            }
        }
    }

    /**
     * @brief determine the number of images in the swapchain
     */

    u32 desired_number_of_swapchain_images = surface_capabilities.minImageCount + 1;

    if (surface_capabilities.maxImageCount > 0 && desired_number_of_swapchain_images > surface_capabilities.maxImageCount) {
        desired_number_of_swapchain_images = surface_capabilities.maxImageCount;
    }

    /**
     * @brief find the transformation of the surface
     */

    VkSurfaceTransformFlagsKHR transform_flags;

    if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        transform_flags = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        transform_flags = surface_capabilities.currentTransform;
    }

    /**
     * @brief find the alpha mode of the surface
     */

    VkCompositeAlphaFlagBitsKHR composite_alpha_flags = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    const std::vector<VkCompositeAlphaFlagBitsKHR> composite_alpha_flags_list = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };

    for (const auto &composite_alpha_flag : composite_alpha_flags_list) {

        if (surface_capabilities.supportedCompositeAlpha & static_cast<u32>(composite_alpha_flag)) {
            composite_alpha_flags = composite_alpha_flag;
            break;
        }
    }

    /**
     * @brief create the swapchain
     */

    VkSwapchainCreateInfoKHR swapchain_create_info = {};

    swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.pNext = NULL;
    swapchain_create_info.surface = _surface;
    swapchain_create_info.minImageCount = desired_number_of_swapchain_images;
    swapchain_create_info.imageFormat = _color._format;
    swapchain_create_info.imageColorSpace = _color._space;
    swapchain_create_info.imageExtent = {extent.width, extent.height};
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.preTransform = static_cast<VkSurfaceTransformFlagBitsKHR>(transform_flags);
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = 0;
    swapchain_create_info.pQueueFamilyIndices = NULL;
    swapchain_create_info.presentMode = sc_present_mode;
    swapchain_create_info.oldSwapchain = old_swapchain;
    swapchain_create_info.clipped = VKE_TRUE;//<< setting clipped to VKE_TRUE allows the implementation to discard rendering outside of the surface area
    swapchain_create_info.compositeAlpha = composite_alpha_flags;

    if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
        swapchain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }

    if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
        swapchain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    VKE_ASSERT(vkCreateSwapchainKHR(_device, &swapchain_create_info, VKE_NULL_PTR, &_swapchain));

    _destroy_buffer();

    if (old_swapchain != VKE_NULL_PTR) {
        _DestroySwapchainKHR(_device, old_swapchain, VKE_NULL_PTR);
        old_swapchain = VKE_NULL_PTR;
    }

    VKE_ASSERT(_GetSwapchainImagesKHR(_device, _swapchain, &_image_count, VKE_NULL_PTR));
    _images.resize(_image_count);
    VKE_ASSERT(_GetSwapchainImagesKHR(_device, _swapchain, &_image_count, _images.data()));
    _buffers.resize(_image_count);

    for (u32 i = 0; i < _image_count; ++i) {
        VkImageViewCreateInfo colorAttachmentView = {};

        colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        colorAttachmentView.pNext = NULL;
        colorAttachmentView.format = _color._format;
        colorAttachmentView.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
        colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        colorAttachmentView.subresourceRange.baseMipLevel = 0;
        colorAttachmentView.subresourceRange.levelCount = 1;
        colorAttachmentView.subresourceRange.baseArrayLayer = 0;
        colorAttachmentView.subresourceRange.layerCount = 1;
        colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
        colorAttachmentView.flags = 0;

        _buffers[i]._image = _images[i];

        colorAttachmentView.image = _buffers[i]._image;

        VKE_ASSERT(vkCreateImageView(_device, &colorAttachmentView, nullptr, &_buffers[i]._view));
    }
}

VkResult vke::priv::VulkanSwapchain::next(VkSemaphore present_semaphore, u32 &image_index)
{
    if (_swapchain) {
        return _AcquireNextImageKHR(_device, _swapchain, UINT64_MAX, present_semaphore, VK_NULL_HANDLE, &image_index);
    }
    return VKE_SUCCESS;
}

VkResult vke::priv::VulkanSwapchain::queue(VkQueue queue, u32 image_index, VkSemaphore wait_semaphore)
{
    VkPresentInfoKHR present_info = {};

    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pNext = VKE_NULL_PTR;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &_swapchain;
    present_info.pImageIndices = &image_index;

    if (wait_semaphore != VKE_NULL_PTR) {
        present_info.pWaitSemaphores = &wait_semaphore;
        present_info.waitSemaphoreCount = 1;
    }

    if (_swapchain) {
        return vkQueuePresentKHR(queue, &present_info);
    }

    return VKE_SUCCESS;
}

void vke::priv::VulkanSwapchain::destroy()
{
    _destroy_buffer();
    if (_swapchain) {
        vkDestroySwapchainKHR(_device, _swapchain, VKE_NULL_PTR);
        _swapchain = VKE_NULL_PTR;
    }
    if (_surface) {
        vkDestroySurfaceKHR(_instance, _surface, VKE_NULL_PTR);
        _surface = VKE_NULL_PTR;
    }
}

/**
 * getters
 */

u32 vke::priv::VulkanSwapchain::getQueueNodeIndex() const
{
    return _queue_index;
}

u32 vke::priv::VulkanSwapchain::getImageCount() const
{
    return _image_count;
}

vke::priv::SwapChainColor &vke::priv::VulkanSwapchain::getColor()
{
    return _color;
}

std::vector<VkImage> &vke::priv::VulkanSwapchain::getImages()
{
    return _images;
}

std::vector<vke::priv::SwapchainBuffers> &vke::priv::VulkanSwapchain::getBuffers()
{
    return _buffers;
}

/**
* private
*/

void vke::priv::VulkanSwapchain::_destroy_buffer()
{
    for (u32 i = 0; i < _buffers.size(); ++i) {
        VKE_SAFE_CLEAN(_buffers[i]._view, vkDestroyImageView(_device, _buffers[i]._view, VKE_NULL_PTR));

        /** NOTE: do not destroy the image it is managed by the swapchain */
        _buffers[i]._view = VK_NULL_HANDLE;
        _buffers[i]._image = VK_NULL_HANDLE;
    }
}
